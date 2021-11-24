#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/model.h"

#include "tensorflow/lite/c/c_api.h"

#include <cassert>
#include <csignal>
#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "portaudio.h"
#include <string>
#include <vector>

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace std;

int labels = 0;

TfLiteTensor* m_input_tensor = nullptr;

int PortAudioCallback(const void* input,
                      void* output,
                      unsigned long frame_count,
                      const PaStreamCallbackTimeInfo* time_info,
                      PaStreamCallbackFlags status_flags,
                      void* user_data);

class PortAudioWrapper {
 public:
  // Constructor.
  PortAudioWrapper(int sample_rate, int num_channels, int bits_per_sample) {
    num_lost_samples_ = 0;
    min_read_samples_ = 320; //sample_rate * 0.1;
    Init(sample_rate, num_channels, bits_per_sample);
  }

  // Reads data from ring buffer.
  template<typename T>
  void Read(std::vector<T>* data) {
    assert(data != NULL);

    // Checks ring buffer overflow.
    if (num_lost_samples_ > 0) {
      std::cerr << "Lost " << num_lost_samples_ << " samples due to ring"
          << " buffer overflow." << std::endl;
      num_lost_samples_ = 0;
    }

    ring_buffer_size_t num_available_samples = 0;
    while (true) {
      num_available_samples =
          PaUtil_GetRingBufferReadAvailable(&pa_ringbuffer_);
      if (num_available_samples >= min_read_samples_) {
        break;
      }
      Pa_Sleep(5);
    }

    // Reads data.
    num_available_samples = PaUtil_GetRingBufferReadAvailable(&pa_ringbuffer_);
    data->resize(num_available_samples);
    ring_buffer_size_t num_read_samples = PaUtil_ReadRingBuffer(
        &pa_ringbuffer_, data->data(), num_available_samples);
    if (num_read_samples != num_available_samples) {
      std::cerr << num_available_samples << " samples were available,  but "
          << "only " << num_read_samples << " samples were read." << std::endl;
    }
  }

  int Callback(const void* input, void* output,
               unsigned long frame_count,
               const PaStreamCallbackTimeInfo* time_info,
               PaStreamCallbackFlags status_flags) {
    // Input audio.
    ring_buffer_size_t num_written_samples =
        PaUtil_WriteRingBuffer(&pa_ringbuffer_, input, frame_count);
    num_lost_samples_ += frame_count - num_written_samples;
    return paContinue;
  }

  ~PortAudioWrapper() {
    Pa_StopStream(pa_stream_);
    Pa_CloseStream(pa_stream_);
    Pa_Terminate();
    PaUtil_FreeMemory(ringbuffer_);
  }

 private:
  // Initialization.
  bool Init(int sample_rate, int num_channels, int bits_per_sample) {
    // Allocates ring buffer memory.
    int ringbuffer_size = 16384;
    ringbuffer_ = static_cast<char*>(
        PaUtil_AllocateMemory(bits_per_sample / 8 * ringbuffer_size));
    if (ringbuffer_ == NULL) {
      std::cerr << "Fail to allocate memory for ring buffer." << std::endl;
      return false;
    }

    // Initializes PortAudio ring buffer.
    ring_buffer_size_t rb_init_ans =
        PaUtil_InitializeRingBuffer(&pa_ringbuffer_, bits_per_sample / 8,
                                    ringbuffer_size, ringbuffer_);
    if (rb_init_ans == -1) {
      std::cerr << "Ring buffer size is not power of 2." << std::endl;
      return false;
    }

    // Initializes PortAudio.
    PaError pa_init_ans = Pa_Initialize();
    if (pa_init_ans != paNoError) {
      std::cerr << "Fail to initialize PortAudio, error message is \""
          << Pa_GetErrorText(pa_init_ans) << "\"" << std::endl;
      return false;
    }

    PaError pa_open_ans;
    if (bits_per_sample == 8) {
      pa_open_ans = Pa_OpenDefaultStream(
          &pa_stream_, num_channels, 0, paUInt8, sample_rate,
          320, PortAudioCallback, this);
    } else if (bits_per_sample == 16) {
      pa_open_ans = Pa_OpenDefaultStream(
          &pa_stream_, num_channels, 0, paInt16, sample_rate,
          320, PortAudioCallback, this);
    } else if (bits_per_sample == 32) {
      pa_open_ans = Pa_OpenDefaultStream(
          &pa_stream_, num_channels, 0, paInt32, sample_rate,
          320, PortAudioCallback, this);
    } else {
      std::cerr << "Unsupported BitsPerSample: " << bits_per_sample
          << std::endl;
      return false;
    }
    if (pa_open_ans != paNoError) {
      std::cerr << "Fail to open PortAudio stream, error message is \""
          << Pa_GetErrorText(pa_open_ans) << "\"" << std::endl;
      return false;
    }

    PaError pa_stream_start_ans = Pa_StartStream(pa_stream_);
    if (pa_stream_start_ans != paNoError) {
      std::cerr << "Fail to start PortAudio stream, error message is \""
          << Pa_GetErrorText(pa_stream_start_ans) << "\"" << std::endl;
      return false;
    }
    return true;
  }

 private:
  // Pointer to the ring buffer memory.
  char* ringbuffer_;

  // Ring buffer wrapper used in PortAudio.
  PaUtilRingBuffer pa_ringbuffer_;

  // Pointer to PortAudio stream.
  PaStream* pa_stream_;

  // Number of lost samples at each Read() due to ring buffer overflow.
  int num_lost_samples_;

  // Wait for this number of samples in each Read() call.
  int min_read_samples_;
};

int PortAudioCallback(const void* input,
                      void* output,
                      unsigned long frame_count,
                      const PaStreamCallbackTimeInfo* time_info,
                      PaStreamCallbackFlags status_flags,
                      void* user_data) {
  PortAudioWrapper* pa_wrapper = reinterpret_cast<PortAudioWrapper*>(user_data);
  pa_wrapper->Callback(input, output, frame_count, time_info, status_flags);
  return paContinue;
}

void SignalHandler(int signal){
  std::cerr << "Caught signal " << signal << ", terminating..." << std::endl;
  exit(0);
}

void run_model(){
  //Initialize Port Audio
	PortAudioWrapper pa_wrapper(16000, 1, 16);
	std::vector<int16_t> data;
  

	vector <float> microphone_float32_data;

  TfLiteModel* model = TfLiteModelCreateFromFile("widya_wakeword_model_lite.tflite");

  TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
  
  TfLiteInterpreter* interpreter = TfLiteInterpreterCreate(model, options);
  TfLiteInterpreterAllocateTensors(interpreter);
  
  int input_tensor_count = TfLiteInterpreterGetInputTensorCount(interpreter);
  int output_tensor_count = TfLiteInterpreterGetOutputTensorCount(interpreter);

  std::cout << "input_tensor_count : " << input_tensor_count << std::endl;
  std::cout << "output_tensor_count : " << output_tensor_count << std::endl;

	TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);
  const TfLiteTensor* output_tensor = TfLiteInterpreterGetOutputTensor(interpreter, 0);

  
  int input_1_x = input_tensor->dims->data[0];
	int input_1_y = input_tensor->dims->data[1];
  int input_3 = input_tensor->dims->data[2];
  int input_4 = input_tensor->dims->data[3];
  int input_5 = input_tensor->dims->data[4];
  int input_6 = input_tensor->dims->data[5];
  int input_7 = input_tensor->dims->data[6];
  int input_8 = input_tensor->dims->data[7];
  

  int output_1 = output_tensor->dims->data[0];
	int output_2 = output_tensor->dims->data[1];
  int output_3 = output_tensor->dims->data[2];
  int output_4 = output_tensor->dims->data[3];
  int output_5 = output_tensor->dims->data[4];
  int output_6 = output_tensor->dims->data[5];
  int output_7 = output_tensor->dims->data[6];
  int output_8 = output_tensor->dims->data[7];
  
  cout<<"Input Data Dimension ("<<input_1_x<<","<<input_1_y<<","<<input_3<<","<<input_4<<","<<input_5<<","<<input_6<<","<<input_7<<")"<<endl;

  cout<<"Output Data Dimension ("<<output_1<<","<<output_2<<","<<output_3<<","<<output_4<<","<<output_5<<","<<output_6<<","<<output_7<<")"<<endl;
	
  vector<int> dimension_state;

  for(int i=0; i<input_tensor_count; i++){
    cout<<"Set Init State Input "<<i<<endl;
    TfLiteTensor* tensor = TfLiteInterpreterGetInputTensor(interpreter, i);
    int size = tensor->dims->size;
    int dimension_total = 1;
    for(int j=0;j<size;j++){
			if(j==0){
				cout<<"Data Dimensions ("<<tensor->dims->data[j];
			}
			else{
				cout<<", "<<tensor->dims->data[j];
			}
			dimension_total = dimension_total * tensor->dims->data[j];
		}
    cout<<")"<<endl;
		cout<<"Dimension Total "<<dimension_total<<endl;
    dimension_state.push_back(dimension_total);
    float *input_data_ptr = (float *)TfLiteTensorData(tensor);
		for(int k=0; k<dimension_total; k++){
			*(input_data_ptr) = 0;
			input_data_ptr++;
		}
		std::this_thread::sleep_for(0.1s);
  }

  while (1) {
    pa_wrapper.Read(&data);
		if (data.size() == 320){
			microphone_float32_data.clear();
			for(int i=0; i<data.size(); i++){
				microphone_float32_data.push_back(data[i] * (1.0 / 32768));
			}

		//Input Microphone Data to Model
    TfLiteTensor* input_audio_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);
    float *input_data_1_ptr = (float *)TfLiteTensorData(input_audio_tensor);

		for(int i=0; i<input_1_y*input_1_x; i++){
			*(input_data_1_ptr) = microphone_float32_data[i];
			input_data_1_ptr++; 
		}

    TfLiteInterpreterInvoke(interpreter);

    const TfLiteTensor* output_keyword = nullptr;
		output_keyword = TfLiteInterpreterGetOutputTensor(interpreter, 0);
    
		auto output_data = output_keyword->data.f;

		//inference predict result, num of label = 5
		for(int i=0; i<5; i++){
			cout<<output_data[i]<<" | "; 
      // cout<<" looping";
      if (output_data[2] > 5)
      {
        labels = 2;
      }
      else if (output_data[3] > 5)
      {
        labels = 3;
      }
      else if (output_data[4] > 5)
      {
        labels = 4;
      }   
		}
    if (labels == 2)
    {
      cout<<"Halo widya Detected!" << endl;  
      labels = 0;
    }
    else if (labels == 3)
    {
      cout<<"Hai widya Detected!" << endl;  
      labels = 0;
    }
    else if (labels == 4)
    {
      cout<<"Stop widya Detected!" << endl;  
      labels = 0;
    }
		cout<<endl;
    
    //update next input state from current output state for next inference

		for(int i=1; i<input_tensor_count; i++){
      float* input_data_update_ptr = (float *)TfLiteTensorData(TfLiteInterpreterGetInputTensor(interpreter, i));
			const TfLiteTensor *output_state = nullptr;
			output_state = TfLiteInterpreterGetOutputTensor(interpreter, i);
			auto output_data_state = output_state->data.f;

			for(int j=0; j<dimension_state[i]; j++){
				*(input_data_update_ptr) = output_data_state[j];
				input_data_update_ptr++;
			}
		}
   }
  }
}

int main(int argc, char** argv) {
	run_model();
  return 0;
}
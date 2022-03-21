#include "../include.cpp"

typedef struct  WAV_HEADER{
    char                RIFF[4];        // RIFF Header      Magic header
    unsigned long       ChunkSize;      // RIFF Chunk Size  
    char                WAVE[4];        // WAVE Header      
    char                fmt[4];         // FMT header       
    unsigned long       Subchunk1Size;  // Size of the fmt chunk                                
    unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
    unsigned long       SamplesPerSec;  // Sampling Frequency in Hz                             
    unsigned long       bytesPerSec;    // bytes per second 
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
    unsigned short      bitsPerSample;  // Number of bits per sample      
    char                Subchunk2ID[4]; // "data"  string   
    unsigned long       Subchunk2Size;  // Sampled data length    

}wav_hdr; 

wav_hdr wavHeader;
int filelength = 0;

int getFileSize(FILE* inFile){
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);
    fileSize = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

int MusicInfo(char * filePath){
    int headerSize = sizeof(wav_hdr);
    FILE* wavFile = fopen(filePath, "r");
    if (wavFile == nullptr) {
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return 0;
    }
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    if (bytesRead > 0) {
        //Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = 4096;
        int8_t* buffer = new int8_t[BUFFER_SIZE];
        std::vector<int8_t> data;
        while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0) data.push_back(bytesRead);
        delete [] buffer;
        buffer = nullptr;
        filelength = getFileSize(wavFile);
    }
    fclose(wavFile);
}


void MusicMaker(){
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { MusicInfo("/home/redragon57/Documents/git/SuperSoft/OBLXKQ - EUPHORIA SECRET.wav"); }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (filelength != 0){
        std::string temp = "File is                    :" + std::to_string(filelength)+ " bytes.";
        ImGui::Text("%s",temp.c_str());
        ImGui::Text("%s","RIFF header                :" + wavHeader.RIFF[0] + wavHeader.RIFF[1] + wavHeader.RIFF[2] + wavHeader.RIFF[3]);
        ImGui::Text("%s","WAVE header                :" + wavHeader.WAVE[0] + wavHeader.WAVE[1] + wavHeader.WAVE[2] + wavHeader.WAVE[3]);
        ImGui::Text("%s","FMT                        :" + wavHeader.fmt[0] + wavHeader.fmt[1] + wavHeader.fmt[2] + wavHeader.fmt[3]);
        ImGui::Text("%s","Data size                  :" + wavHeader.ChunkSize);

        // Display the sampling Rate from the header
        ImGui::Text("%s","Sampling Rate              :" + wavHeader.SamplesPerSec);
        ImGui::Text("%s","Number of bits used        :" + wavHeader.bitsPerSample );
        ImGui::Text("%s","Number of channels         :" + wavHeader.NumOfChan);
        ImGui::Text("%s","Number of bytes per second :" + wavHeader.bytesPerSec);
        ImGui::Text("%s","Data length                :" + wavHeader.Subchunk2Size);
        ImGui::Text("%s","Audio Format               :" + wavHeader.AudioFormat);
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        ImGui::Text("%s","Block align                :" + wavHeader.blockAlign);
        ImGui::Text("%s","Data string                :" + wavHeader.Subchunk2ID[0] + wavHeader.Subchunk2ID[1] + wavHeader.Subchunk2ID[2] + wavHeader.Subchunk2ID[3]);
    }
}

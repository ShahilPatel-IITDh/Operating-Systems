#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <sys/wait.h>
#include <atomic>

using namespace std;

class Pixel{
    public: 
        int red;
        int green;
        int blue;

        // Pixel(int red, int green, int blue){
        void setRed(int red){
            this->red = red;
        }

        int getRed(){
            return this->red;
        }

        void setGreen(int green){
            this->green = green;
        }

        int getGreen(){
            return this->green;
        }

        void setBlue(int blue){
            this->blue = blue;
        }

        int getBlue(){
            return this->blue;
        }
};

class fileInfo{
    public:
        int height;
        int width;
        int maxASCII;
        // PPM Version is stored as a string, as it can be P3 or P6.
        char PPM_VERSION[100];

        void setHeight(int height){
            this->height = height;
        }
        int getHeight(){
            return this->height;
        }

        void setWidth(int width){
            this->width = width;
        }

        int getWidth(){
            return this->width;
        }

        void setMaxASCII(int maxASCII){
            this->maxASCII = maxASCII;
        }

        int getMaxASCII(){
            return this->maxASCII;
        }

        void setPPM_VERSION(char PPM_VERSION[100]){
            strcpy(this->PPM_VERSION, PPM_VERSION);
        }

        char* getPPM_VERSION(){
            return this->PPM_VERSION;
        }
};

int getNewColour(int colour, int blur){
    int newColour = colour * (0.5/blur);
    return newColour;
}

void HorizontalBlur(char *prevInput, int fileDescriptor1[2], int fileDescriptor2[2], int pid){
    if (pid > 0){
        return;
    }

    else if(pid == -1){
        cout<<"Error in creating child process"<<endl;
        exit(1);
    }

    //for the pipe

    // getData will be used to store the 9 pixels that will be read from the pipe.
    // The 9 pixels will be stored in the following order:
    Pixel getData[9];

    // The head array will be used to store the header information of the image.
    fileInfo head[1];

    // Read the header information from the pipe, and store it in the head array, which is of size head.
    read(fileDescriptor2[0], head, sizeof(head));

    // The values array will be used to store the pixel values of the image.
    vector<vector<Pixel>> values(head[0].getHeight(), vector<Pixel>(head[0].getWidth()));

    // The tvalues array will be used to store the pixel values of the image.
    vector<vector<Pixel>> tvalues(head[0].getHeight(), vector<Pixel>(head[0].getWidth()));

    int blurAmount = 50;
    for (int i = 0; i <= head[0].getHeight() - 3; i+=3){
        // We jump by 3 because we are reading 3 rows at a time.
        for (int j = 0; j <= head[0].getWidth() - 3; j+=3){
            // We jump by 3 because we are reading 3 columns at a time.

            // Read the 9 pixels from the pipe, and store them in the getData array.
            read(fileDescriptor1[0], getData, sizeof(getData));

            values[i][j] = getData[0];
            values[i][j + 1] = getData[1];
            values[i][j + 2] = getData[2];
            values[i + 1][j] = getData[3];
            values[i + 1][j + 1] = getData[4];
            values[i + 1][j + 2] = getData[5];
            values[i + 2][j] = getData[6];
            values[i + 2][j + 1] = getData[7];
            values[i + 2][j + 2] = getData[8];
        }
    }

    for (int i = 0; i < head[0].getHeight(); i++){
        for (int j = 0; j < head[0].getWidth(); j++){

            int r = values[i][j].getRed();
            int colourRed = r/2;

            int g = values[i][j].getGreen();
            int colourGreen = g/2;

            int b = values[i][j].getBlue();
            int colourBlue = b/2;

            if((head[0].getWidth()-j)<blurAmount){

                // newBlurAmount is the amount of pixels that are left in the row.
                int newBlurAmount = head[0].getWidth()-j;

                for(int k = j+1; k < head[0].getWidth(); k++){

                    int r = values[i][k].getRed();
                    colourRed += getNewColour(r, newBlurAmount);

                    int g = values[i][k].getGreen();
                    colourGreen += getNewColour(g, newBlurAmount);
                    
                    int b = values[i][k].getBlue();
                    colourBlue += getNewColour(b, newBlurAmount);   
                }
                values[i][j].setRed(colourRed);
                values[i][j].setBlue(colourBlue);
                values[i][j].setGreen(colourGreen);
                continue;
            }

            for (int k = 1; k < blurAmount; k++){

                int r = values[i][j+k].getRed();
                colourRed += getNewColour(r, blurAmount);
                
                int g = values[i][j+k].getGreen();
                colourGreen += getNewColour(g, blurAmount);

                int b = values[i][j+k].getBlue();
                colourBlue += getNewColour(b, blurAmount);
            }
            values[i][j].setRed(colourRed);
            values[i][j].setBlue(colourBlue);
            values[i][j].setGreen(colourGreen);
        }
    }

    // Write the updated pixel values to the pipe.
    FILE *outputImage = fopen(prevInput, "w");


    // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.

    fprintf(outputImage, "%s\n%d %d\n%d\n", head[0].getPPM_VERSION(), head[0].getWidth(), head[0].getHeight(), head[0].getMaxASCII());

    // Printing the updated pixel values to the file.
    int initialVal = head[0].getHeight() - 1;
    for (int i = initialVal; i >= 0; i--){
        // Printing RGB pixel values from above updated image values.
        for (int j = 0; j <= head[0].getWidth() - 1; j++){
            // Printing RGB pixel values from above updated image values.
            fprintf(outputImage, "%d ", values[i][j].red);            
            fprintf(outputImage, "%d ", values[i][j].green);
            fprintf(outputImage, "%d ", values[i][j].blue);
        }
        fprintf(outputImage, "\n");
    }
    fclose(outputImage);
    exit(0);
}

// Function to converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void RGBtoGrayScale(char *input, int fileDescriptor1[2], int fileDescriptor2[2], int pid)
{
    if (pid > 0){
        return;
    }
    if(pid == -1){
        cout<<"Error in creating child process"<<endl;
    }
    //open ppm file and read
    int height;
    int width;
    int maxASCII;
    char PPM_VERSION[100];

    // Read input file
    FILE *input_image = fopen(input, "r");
    // fileInfo is a struct that stores the PPM Version, Width, Height and Maximum Ascii value allowed.

    fileInfo head[1];
    fscanf(input_image, "%s%d%d%d", head->PPM_VERSION, &width, &height, &maxASCII); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.

    // set the values of the fileInfo object 'head'.
    head[0].setHeight(height);
    head[0].setWidth(width);
    head[0].setMaxASCII(maxASCII);

    // Write the fileInfo object 'head' to the pipe.
    write(fileDescriptor2[1], head, sizeof(fileInfo));
    
    vector<vector<Pixel>> values(height, vector<Pixel>(width)); //Vector for reading and storing pixels as a values.

    int red;
    int green;
    int blue;

    for (int i = height-1; i >= 0; i--){

        for (int j = 0; j <= width - 1; j++)
        {   //Storing RGB pixel values into above created values.
            fscanf(input_image, "%d%d%d", &red, &green, &blue);            
            values[i][j].setRed(red);
            values[i][j].setGreen(green);
            values[i][j].setBlue(blue);
        }
    }

    fclose(input_image);
    Pixel forwardData[9];

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            int colourRed = values[i][j].getRed();
            int colourGreen = values[i][j].getGreen();
            int colourBlue = values[i][j].getBlue();

            int luminous = (colourBlue*0.114) + (colourGreen*0.587) + (colourRed*0.299);

            int newRed = luminous;
            int newGreen = luminous;
            int newBlue = luminous;

            values[i][j].setRed(newRed);
            values[i][j].setGreen(newGreen);
            values[i][j].setBlue(newBlue);
        }
    }

    for (int i = 0; i<=height-3; i += 3){
        for (int j = 0; j<=width-3; j += 3){

            forwardData[0] = values[i][j];
            forwardData[1] = values[i][j + 1];
            forwardData[2] = values[i][j + 2];
            forwardData[3] = values[i + 1][j];
            forwardData[4] = values[i + 1][j + 1];
            forwardData[5] = values[i + 1][j + 2];
            forwardData[6] = values[i + 2][j];
            forwardData[7] = values[i + 2][j + 1];
            forwardData[8] = values[i + 2][j + 2];
            write(fileDescriptor1[1], forwardData, sizeof(forwardData));
        }
    }
    exit(0);
}

int main(int argc, char* argv[]){

    //Creating two pipes for multi-directional communication
    int fileDescriptor1[2];
    int data1;
    data1 = pipe(fileDescriptor1);
    if (data1 == -1){
        perror("pipe");
    }

    int fileDescriptor2[2];
    int data2;

    data2 = pipe(fileDescriptor2);

    if (data2 == -1){
        perror("pipe");
    }

    auto beginStamp = chrono::high_resolution_clock::now(); // Starting the clock
    
    // Calling the T1 and T2 functions
    // RGBtoGrayScale function = T1
    // HorizontalBlur function = T2
    RGBtoGrayScale(argv[1], fileDescriptor1, fileDescriptor2, fork()); 
    HorizontalBlur(argv[2], fileDescriptor1, fileDescriptor2, fork()); 

    // Waiting for T1 and T2 to finish
    wait(NULL);
    wait(NULL);

    auto endStamp = chrono::high_resolution_clock::now(); //Stopping the clock

    // Calculating the time taken by T1 and T2.
    auto duration = chrono::duration_cast<chrono::microseconds>(endStamp - beginStamp); // Calculating the time taken by T1 and T2.
    cout << "Time: " << duration.count() << " microseconds"<< endl;
    return 0;
}
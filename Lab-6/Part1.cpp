#include <bits/stdc++.h>
#include <chrono>
#include <vector>

using namespace std;

class Pixel{
    public: 
        int red;
        int green;
        int blue;

        // Pixel(int red, int green, int blue){
        //     this->red = red;
        //     this->green = green;
        //     this->blue = blue;
        // }

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


// Function to converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void RGBtoGrayScale(int width, int height, vector<vector<Pixel>> &matrix){

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int colourRed = matrix[i][i].getRed();
            int colourGreen = matrix[i][j].getGreen();
            int colourBlue = matrix[i][j].getBlue();
            matrix[i][j].setRed((colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587));
            matrix[i][j].setGreen((colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587));
            matrix[i][j].setBlue((colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587));
        }
    }
}

int main(int argc, char* argv[]){

    if(argc != 3){
        cout << "Please enter the input and output file names." << endl;
        return 0;
    }

    int width;
    int height;
    int maxASCII;
    char ppmVersion[10];
    FILE* inputImage = fopen(argv[1], "r");

    if (inputImage == NULL) {
        cout << "Input Image not available\n";
        exit(1);
    }

    FILE* outputImage = fopen(argv[2], "w");

    // Read from the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    fscanf(inputImage, "%s%d%d%d", ppmVersion, &width, &height, &maxASCII);

    // fscanf(inputImage, "%s", ppmVersion);
    // fscanf(inputImage, "%d %d", &width, &height);
    // fscanf(inputImage, "%d", &maxASCII);

    // make a vector to store the pixels 
    // each pixel will be of type 'pixel'
    vector<vector<Pixel>> matrix(height, vector<Pixel>(width));

    int red;
    int green;
    int blue;

    // read the pixels from the file and store them in the vector
    for(int i=height-1; i>=0; i--){
        for(int j=0; j<width; j++){
            fscanf(inputImage, "%d%d%d", &red, &green, &blue);
            matrix[i][j].setRed(red);
            matrix[i][j].setGreen(green);
            matrix[i][j].setBlue(blue);
        }
    }
    fclose(inputImage);

    auto beginStamp = chrono::high_resolution_clock::now(); // Starting the clock
    RGBtoGrayScale(width, height, matrix);
    auto endStamp = chrono::high_resolution_clock::now(); //Stopping the clock
    auto duration = chrono::duration_cast<chrono::microseconds>(endStamp - beginStamp);//Calculating the time taken by T1 and T2
    cout<<"Time Taken: "<<duration.count()<<" microseconds"<<endl;

    // write the pixels to the output file
    fprintf(outputImage, "%s\n%d %d\n%d\n", ppmVersion, width, height, maxASCII);
    for(int i=height-1; i>=0; i--){
        for(int j=0; j<width; j++){
            fprintf(outputImage, "%d", matrix[i][j].getRed());
            fprintf(outputImage, "%d", matrix[i][j].getGreen());
            fprintf(outputImage, "%d", matrix[i][j].getBlue());
        }
        fprintf(outputImage, "\n");
    }
    fclose(outputImage);
    return 0;
}
#include <bits/stdc++.h>
#include <chrono>
#include <vector>

using namespace std;

class Pixel{
    public: 
        int red;
        int green;
        int blue;

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

int getNewColour(int colour, int blur){
    int newColour = colour * (0.5/blur);
    return newColour;
}

void HorizontalBlur(int width, int height, vector<vector<Pixel>> &matrix){
    
    int blurAmount = 30;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){

            int colourRed = matrix[i][j].getRed()/2;
            int colourGreen = matrix[i][j].getGreen()/2;
            int colourBlue = matrix[i][j].getBlue()/2;

            if((width-j)<blurAmount){

                int newBlurAmount = width-j;

                for(int k = j+1; k < width; k++){
                    colourRed += getNewColour(matrix[i][k].getRed(), newBlurAmount);
                    // colourRed += (matrix[i][k].getRed() * (0.5/newBlurAmount));
                    colourGreen += getNewColour(matrix[i][k].getGreen(), newBlurAmount);
                    // colourGreen += (matrix[i][k].getGreen() * (0.5/newBlurAmount));
                    colourBlue += getNewColour(matrix[i][k].getBlue(), newBlurAmount);
                    // colourBlue += (matrix[i][k].getBlue() * (0.5/newBlurAmount));   
                }

                matrix[i][j].setRed(colourRed);
                matrix[i][j].setBlue(colourBlue);
                matrix[i][j].setGreen(colourGreen);
                continue;
            }

            for (int k = 1; k < blurAmount; k++){
                colourRed += getNewColour(matrix[i][j+k].getRed(), blurAmount);
                // colourRed += matrix[i][j+k].getRed() * (0.5/blurAmount);
                colourGreen += getNewColour(matrix[i][j+k].getGreen(), blurAmount);
                // colourGreen += matrix[i][j+k].getGreen() * (0.5/blurAmount);
                colourBlue += getNewColour(matrix[i][j+k].getBlue(), blurAmount);
                // colourBlue += matrix[i][j+k].getBlue() * (0.5/blurAmount);
            }

            matrix[i][j].setRed(colourRed);
            matrix[i][j].setBlue(colourBlue);
            matrix[i][j].setGreen(colourGreen);
        }
    }
}

// Function to converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void RGBtoGrayScale(int width, int height, vector<vector<Pixel>> &matrix){

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int colourRed = matrix[i][i].getRed();
            int colourGreen = matrix[i][j].getGreen();
            int colourBlue = matrix[i][j].getBlue();

            // weighted average of red, green and blue is calculated and then assigned to all three values
            // this is done to convert the image to grayscale
            // wighted average = (0.299 * red) + (0.587 * green) + (0.114 * blue)
            int newRed = (colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587);
            int newGreen = (colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587);
            int newBlue = (colourBlue * 0.114) + (colourRed * 0.299) + (colourGreen * 0.587);

            matrix[i][j].setRed(newRed);
            matrix[i][j].setGreen(newGreen);
            matrix[i][j].setBlue(newBlue);
        }
    }
}

void printImage(int width, int height, vector<vector<Pixel>> &matrix, FILE* outputImage){
    for(int i=height-1; i>=0; i--){
        for(int j=0; j<width; j++){
            fprintf(outputImage, "%d ", matrix[i][j].getRed());
            fprintf(outputImage, "%d ", matrix[i][j].getGreen());
            fprintf(outputImage, "%d ", matrix[i][j].getBlue());
        }
        fprintf(outputImage, "\n");
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
    char ppmVersion[3];
    FILE* inputImage = fopen(argv[1], "r");

    if (inputImage == NULL) {
        cout << "Input Image not available\n";
        exit(1);
    }

    FILE* outputImage = fopen(argv[2], "w");

    // Read from the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    fscanf(inputImage, "%s%d%d%d", ppmVersion, &width, &height, &maxASCII);

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

    // call the function to convert the image to grayscale (T1 function)
    RGBtoGrayScale(width, height, matrix);
    // call the function to apply the horizontal blur (T2 function)
    HorizontalBlur(width, height, matrix);

    auto endStamp = chrono::high_resolution_clock::now(); //Stopping the clock
    auto duration = chrono::duration_cast<chrono::microseconds>(endStamp - beginStamp);//Calculating the time taken by T1 and T2
    cout<<"Time Taken: "<<duration.count()<<" microseconds"<<endl;

    // write the pixels to the output file
    fprintf(outputImage, "%s\n%d %d\n%d\n", ppmVersion, width, height, maxASCII);

    // call the function to print the image to the output file
    printImage(width, height, matrix, outputImage);

    fclose(outputImage);
    return 0;
}
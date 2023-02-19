#include <bits/stdc++.h>
#include <chrono>
#include <vector>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h> //for fork()
#include <fcntl.h>

using namespace std;

#define SEM_NAME "/s"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1

//Creating struct structure for reading pixels easily in ppm3


// Class is given segmentation fault
struct Pixel{
    int red;
    int green;
    int blue;
};

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void RGBtoGrayScale(key_t key, int height, int width, int pid){
    
    if (pid > 0){
        return;
    }

    if(pid == -1){
        cout<<"Error in fork"<<endl;
    }

    int shmid = shmget(key, sizeof(struct Pixel) * height * width, 0666 | IPC_CREAT);
    
    if(shmid == -1){
        cout<<"Error in shmget"<<endl;
    }

    struct Pixel *values;

    values = (struct Pixel *)shmat(shmid, NULL, 0);

    if(values == (void *)-1){
        cout<<"Error in shmat"<<endl;
    }

    //access named semaphore
    sem_t *s = sem_open(SEM_NAME, O_RDWR);

    if(s == SEM_FAILED){
        cout<<"Error in sem_open"<<endl;
    }
        
    for (int p = 0; p < height; p++){
        sem_wait(s);

        for (int q = 0; q < width; q++){
            struct Pixel temp;

            temp = values[p * width + q];

            int redColour = temp.red;
            int greenColour = temp.green;
            int blueColour = temp.blue;

            int lumious = (blueColour * 0.114) + (redColour * 0.299) + (greenColour * 0.587);

            temp.red = lumious;
            temp.green = lumious;
            temp.blue = lumious;
            values[p * width + q] = temp;

        }
        sem_post(s);
    }
}

int getColour(int colour, int blurAmount){
    int blur = 0.5/blurAmount;
    int newColour = colour * blur;
    return newColour;
}

//This function does a horizontal blur which makes a photo as if it is in motion.
void HorizontalBlur(key_t key, int h, int w, int pid){

    if (pid > 0){
        return;
    }

    if(pid == -1){
        cout<<"Error in fork"<<endl;
    }

    int shmid = shmget(key, sizeof(struct Pixel) * h * w, 0666 | IPC_CREAT);

    if(shmid == -1){
        cout<<"Error in shmget"<<endl;
    }

    struct Pixel *values;
    values = (struct Pixel *)shmat(shmid, NULL, 0);
    
    if(values == (void *)-1){
        cout<<"Error in shmat"<<endl;
    }

    struct Pixel temp;

    vector<vector<Pixel>> matrixValues(h, vector<Pixel>(w));
    
    sem_t *s = sem_open(SEM_NAME, O_RDWR);

    int blurAmount = 50;

    for (int p = 0; p < h; p++){
        sem_wait(s);
        for (int q = 0; q < w; q++){

            int redColour;
            int valueRed = values[(p * w) + q].red;

            redColour = valueRed/2;

            int greenColour;
            int valueGreen = values[(p * w) + q].green;

            greenColour = valueGreen/2;

            int blueColour;
            int valueBlue = values[(p * w) + q].blue;

            blueColour = valueBlue/2;

            if((w-q) < blurAmount){

                int newBlurAmount = w-q;

                for(int u = q+1; u < w; u++){
                    
                    int idx = (p*w)+u;

                    int redArg = values[idx].red;
                    redColour += getColour(redArg, newBlurAmount);

                    int greenArg = values[idx].green;
                    greenColour += getColour(greenArg, newBlurAmount);

                    int blueArg = values[idx].blue;
                    blueColour += getColour(blueArg, newBlurAmount);
                }

                matrixValues[p][q].red = redColour;
                matrixValues[p][q].blue = blueColour;
                matrixValues[p][q].green = greenColour;
                continue;
            }

            for (int i = 1; i < blurAmount; i++){
                
                int idx = (p*w)+(q+i);
                
                int redArg = values[idx].red;
                redColour += getColour(redArg, blurAmount);

                int greenArg = values[idx].green;
                greenColour += getColour(greenArg, blurAmount);

                int blueArg = values[idx].blue;
                blueColour += getColour(blueArg, blurAmount);
            }

            matrixValues[p][q].red = redColour;
            matrixValues[p][q].blue = blueColour;
            matrixValues[p][q].green = greenColour;            
        }
        sem_post(s);
    }

    for (int i = h-1; i >= 0; i--){
        for (int j = 0; j <= w - 1; j++){
            values[(i * w) + j] = matrixValues[i][j];
        }
    }
}

int main(int argc, char *argv[]){

    int width;
    int height;
    int maxAscii;

    char PPM_VERSION[10];
    FILE *input_image = fopen(argv[1], "r");

    fscanf(input_image, "%s%d%d%d", PPM_VERSION, &width, &height, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
    
    struct Pixel *values;

    key_t key = 0x1234;
    int shmid = shmget(key, sizeof(struct Pixel) * (height) * width, 0666 | IPC_CREAT);

    values = (struct Pixel *)shmat(shmid, NULL, 0);

    struct Pixel temp;
    
    //vector<vector<Pixel>> values(h, vector<Pixel>(w)); //Vector for reading and storing pixels as a matrix. 
    int red, green, blue;

    for (int i = height-1; i >= 0; i--){
        for (int j=0; j<width; j++){   
            //Storing RGB pixel values into above created matrix.
            fscanf(input_image, "%d%d%d", &red, &green, &blue);
            temp.red = red;
            temp.green = green;
            temp.blue = blue;
            values[(i * width) + j] = temp;
        }
    }

    fclose(input_image);

    auto beginStamp = chrono::high_resolution_clock::now(); // Starting the clock

    sem_t *s = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE); //named semaphore

    RGBtoGrayScale(key, height, width, fork());
    HorizontalBlur(key, height, width, fork());

    wait(NULL);
    wait(NULL);

    auto endStamp = chrono::high_resolution_clock::now(); //Stopping the clock

    // Calculating the time taken by T1 and T2.
    auto duration = chrono::duration_cast<chrono::microseconds>(endStamp - beginStamp);

    cout << "Time: " << duration.count() << " microseconds"<< endl;

    FILE *output_image = fopen(argv[2], "w");
    
    fprintf(output_image, "%s\n%d %d\n%d\n", PPM_VERSION, width, height, maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.

    for (int i=height-1; i>=0; i--){
        for (int j=0; j<width; j++){  
             // Printing RGB pixel values from above updated image matrix.
            temp = values[(i * width) + j];

            fprintf(output_image, "%d ", temp.red);
            fprintf(output_image, "%d ", temp.green);
            fprintf(output_image, "%d ", temp.blue);
        }
        fprintf(output_image, "\n");
    }

    fclose(output_image);
    shmdt(values);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}

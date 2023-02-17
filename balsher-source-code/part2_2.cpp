#include <bits/stdc++.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <semaphore.h>
#include <unistd.h> //for fork()
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>

using namespace std;

#define SEM_NAME "/s"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1

//Creating struct structure for reading pixels easily in ppm3

struct one_pixel
{
    int red;
    int green;
    int blue;
};

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void ConvertGrayscale(key_t key, int h, int w, int pid)
{
    if (pid > 0)
        return;

    int shmid = shmget(key, sizeof(struct one_pixel) * h * w, 0666 | IPC_CREAT);
    struct one_pixel *values;
    values = (struct one_pixel *)shmat(shmid, NULL, 0);

    //access named semaphore
    sem_t *s = sem_open(SEM_NAME, O_RDWR);
        
    for (int p = 0; p < h; p++)
    {
        sem_wait(s);
        for (int q = 0; q < w; q++)
        {
            
            struct one_pixel t;
            t = values[p * w + q];
            int colour_red = t.red;
            int colour_green = t.green;
            int colour_blue = t.blue;
            t.red = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
            t.green = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
            t.blue = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
            values[p * w + q] = t;
            
        }
        sem_post(s);
    }
    //cout<<"In gray "<<values->red<<endl;
    // shmdt(NULL);
}

//This function does a horizontal blur which makes a photo as if it is in motion.
void HorizontalBlur(key_t key, int h, int w, int pid)
{
    if (pid > 0)
        return;

    int shmid = shmget(key, sizeof(struct one_pixel) * h * w, 0666 | IPC_CREAT);
    struct one_pixel *values;
    values = (struct one_pixel *)shmat(shmid, NULL, 0);

    struct one_pixel t;

    vector<vector<one_pixel>> tvalues(h, vector<one_pixel>(w));
    
    sem_t *s = sem_open(SEM_NAME, O_RDWR);
    int BLUR_AMOUNT = 50;
    for (int p = 0; p < h; p++)
    {
        sem_wait(s);
        for (int q = 0; q < w; q++)
        {
            int colour_red = values[(p * w) + q].red/2;
            int colour_green = values[(p * w) + q].green/2;
            int colour_blue = values[(p * w) + q].blue/2;
            if((w-q)<BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = w-q;
                for(int u = q+1; u < w; u++)
                {
                    colour_red += values[(p * w) + u].red * (0.5/NEW_BLUR_AMOUNT);
                    colour_green += values[(p * w) + u].green * (0.5/NEW_BLUR_AMOUNT);
                    colour_blue += values[(p * w) + u].blue * (0.5/NEW_BLUR_AMOUNT);   
                }
                tvalues[p][q].red = colour_red;
                tvalues[p][q].blue = colour_blue;
                tvalues[p][q].green = colour_green;
                // values[(p * w) + q] = tvalues[p][q];
                //sem_post(s);
                continue;
            }
            for (int i = 1; i < BLUR_AMOUNT; i++)
            {
                colour_red += values[(p * w) + (q+i)].red * (0.5/BLUR_AMOUNT);
                colour_green += values[(p * w) + (q+i)].green * (0.5/BLUR_AMOUNT);
                colour_blue += values[(p * w) + (q+i)].blue * (0.5/BLUR_AMOUNT);
            }
            tvalues[p][q].red = colour_red;
            tvalues[p][q].blue = colour_blue;
            tvalues[p][q].green = colour_green;
            // values[(p * w) + q] = tvalues[p][q];
            
        }
        sem_post(s);
    }
    for (int i = h-1; i >= 0; i--)
    {
        for (int j = 0; j <= w - 1; j++)
        {
            values[(i * w) + j] = tvalues[i][j];
        }
    }
    //cout<<"In blur values"<<values->red<<endl;
    //cout<<"In blur t"<<tvalues[-1][-1].red<<endl;
    // shmdt(NULL);
}

int main(int argc, char *argv[])
{
    int w, h, maxAscii;
    char PPM_VERSION[10];
    FILE *input_image = fopen(argv[1], "r");
    fscanf(input_image, "%s%d%d%d", PPM_VERSION, &w, &h, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
    
    struct one_pixel *values;

    key_t key = 0x1234;
    int shmid = shmget(key, sizeof(struct one_pixel) * (h)*w, 0666 | IPC_CREAT);

    values = (struct one_pixel *)shmat(shmid, NULL, 0);

    struct one_pixel t;
    
    //vector<vector<one_pixel>> values(h, vector<one_pixel>(w)); //Vector for reading and storing pixels as a matrix. 
    int red, green, blue;
    for (int i = h - 1; i >= 0; i--)
    {
        for (int j = 0; j <= w - 1; j++)
        {   //Storing RGB pixel values into above created matrix.
            fscanf(input_image, "%d%d%d", &red, &green, &blue);
            t.red = red;
            t.green = green;
            t.blue = blue;
            values[(i * w) + j] = t;
        }
    }
    //cout<<"1 "<<values->red<<endl;
    fclose(input_image);

    auto begin = chrono::high_resolution_clock::now(); // Starting the clock

    sem_t *s = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE); //named semaphore

    HorizontalBlur(key, h, w, fork());
    ConvertGrayscale(key, h, w, fork());

    wait(NULL);
    wait(NULL);

    auto end = chrono::high_resolution_clock::now(); //Stopping the clock
    auto taken_time = chrono::duration_cast<chrono::microseconds>(end - begin); // Calculating the time taken by T1 and T2.
    cout << "Time: " << taken_time.count() << " microseconds"<< endl;
    FILE *output_image = fopen(argv[2], "w");
    fprintf(output_image, "%s\n%d %d\n%d\n", PPM_VERSION, w, h, maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    for (int i = h - 1; i >= 0; i--)
    {
        for (int j = 0; j <= w - 1; j++)
        {   // Printing RGB pixel values from above updated image matrix.
            t = values[(i * w) + j];
            fprintf(output_image, "%d ", t.red);
            fprintf(output_image, "%d ", t.green);
            fprintf(output_image, "%d ", t.blue);
        }
        fprintf(output_image, "\n");
    }
    fclose(output_image);
    shmdt(values);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}

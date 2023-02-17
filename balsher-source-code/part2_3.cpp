#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <atomic>

using namespace std;

//Creating struct structure for reading pixels easily in ppm3

struct one_pixel
{
    int red;
    int green;
    int blue;
};

struct file_info
{
    int h;
    int w;
    int maxAscii;
    char PPM_VERSION[100];
};

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void ConvertGrayscale(char *input, int create_pipe[2], int create_pipe2[2], int pid)
{
    if (pid > 0)
    {
        return;
    }

    //open ppm file and read
    int h, w, maxAscii;
    char PPM_VERSION[100];

    FILE *input_image = fopen(input, "r");

    struct file_info head[1];
    fscanf(input_image, "%s%d%d%d", head->PPM_VERSION, &w, &h, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
    
    head[0].h = h;
    head[0].w = w;
    head[0].maxAscii = maxAscii;
    write(create_pipe2[1], head, sizeof(struct file_info));
    
    vector<vector<one_pixel>> values(h, vector<one_pixel>(w)); //Vector for reading and storing pixels as a matrix. 
    int red, green, blue;
    for (int i = h - 1; i >= 0; i--)
    {
        for (int j = 0; j <= w - 1; j++)
        {   //Storing RGB pixel values into above created matrix.
            fscanf(input_image, "%d%d%d", &red, &green, &blue);
            values[i][j].red = red;
            values[i][j].green = green;
            values[i][j].blue = blue;
        }
    }
    fclose(input_image);

    struct one_pixel forward_data[9];
    for (int p = 0; p < h; p++)
    {
        for (int q = 0; q < w; q++)
        {
            int colour_red = values[p][q].red;
            int colour_green = values[p][q].green;
            int colour_blue = values[p][q].blue;
            values[p][q].red = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
            values[p][q].green = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
            values[p][q].blue = (colour_blue * 0.114) + (colour_red * 0.299) + (colour_green * 0.587);
        }
    }
    for (int i = 0; i <= h - 3; i += 3)
    {
        for (int j = 0; j <= w - 3; j += 3)
        {
            forward_data[0] = values[i][j];
            forward_data[1] = values[i][j + 1];
            forward_data[2] = values[i][j + 2];

            forward_data[3] = values[i + 1][j];
            forward_data[4] = values[i + 1][j + 1];
            forward_data[5] = values[i + 1][j + 2];

            forward_data[6] = values[i + 2][j];
            forward_data[7] = values[i + 2][j + 1];
            forward_data[8] = values[i + 2][j + 2];

            write(create_pipe[1], forward_data, sizeof(forward_data));
        }
    }
    // cout << sizeof(values)<<endl;
    // cout <<"1 "<< forward_data[8].red<<endl;
    // cout <<"1 "<< forward_data[8].green<<endl;
    // cout <<"1 "<< forward_data[8].blue<<endl;
    exit(0);
}

//This function does a horizontal blur which makes a photo as if it is in motion.
void HorizontalBlur(char *prev_input, int create_pipe[2], int create_pipe2[2], int pid)
{
    if (pid > 0)
        return;
    //for the pipe
    struct one_pixel get_data[9];

    struct file_info head[1];
    read(create_pipe2[0], head, sizeof(head));

    // cout<<head->h<<" "<<head->w<<" "<<head->maxAscii<<endl;

    vector<vector<one_pixel>> values(head[0].h, vector<one_pixel>(head[0].w));
    vector<vector<one_pixel>> tvalues(head[0].h, vector<one_pixel>(head[0].w));
    // cout<<"YES"<<endl;
    int BLUR_AMOUNT = 50;


    for (int p = 0; p <= head[0].h - 3; p+=3)
    {
        for (int j = 0; j <= head[0].w - 3; j+=3)
        {
            read(create_pipe[0], get_data, sizeof(get_data));
            values[p][j] = get_data[0];
            values[p][j + 1] = get_data[1];
            values[p][j + 2] = get_data[2];

            values[p + 1][j] = get_data[3];
            values[p + 1][j + 1] = get_data[4];
            values[p + 1][j + 2] = get_data[5];

            values[p + 2][j] = get_data[6];
            values[p + 2][j + 1] = get_data[7];
            values[p + 2][j + 2] = get_data[8];
        }
    }
    
    // cout <<"2 "<< values[head[0].h - 3 + 2][head[0].w - 3 + 2].red<<endl;
    // cout <<"2 "<< values[head[0].h - 3 + 2][head[0].w - 3 + 2].green<<endl;
    // cout <<"2 "<< values[head[0].h - 3 + 2][head[0].w - 3 + 2].blue<<endl;
    // cout << sizeof(values)<<endl;
    for (int p = 0; p < head[0].h; p++)
    {
        for (int q = 0; q < head[0].w; q++)
        {
            int colour_red = values[p][q].red/2;
            int colour_green = values[p][q].green/2;
            int colour_blue = values[p][q].blue/2;
            if((head[0].w-q)<BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = head[0].w-q;
                for(int u = q+1; u < head[0].w; u++)
                {
                    colour_red += values[p][u].red * (0.5/NEW_BLUR_AMOUNT);
                    colour_green += values[p][u].green * (0.5/NEW_BLUR_AMOUNT);
                    colour_blue += values[p][u].blue * (0.5/NEW_BLUR_AMOUNT);   
                }
                values[p][q].red = colour_red;
                values[p][q].blue = colour_blue;
                values[p][q].green = colour_green;
                continue;
            }
            for (int i = 1; i < BLUR_AMOUNT; i++)
            {
                colour_red += values[p][q+i].red * (0.5/BLUR_AMOUNT);
                colour_green += values[p][q+i].green * (0.5/BLUR_AMOUNT);
                colour_blue += values[p][q+i].blue * (0.5/BLUR_AMOUNT);
            }
            values[p][q].red = colour_red;
            values[p][q].blue = colour_blue;
            values[p][q].green = colour_green;
        }
    }
    FILE *output_image = fopen(prev_input, "w");
    fprintf(output_image, "%s\n%d %d\n%d\n", head[0].PPM_VERSION, head[0].w, head[0].h, head[0].maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    for (int i = head[0].h - 1; i >= 0; i--)
    {
        for (int j = 0; j <= head[0].w - 1; j++)
        {   // Printing RGB pixel values from above updated image matrix.
            fprintf(output_image, "%d ", values[i][j].red);
            fprintf(output_image, "%d ", values[i][j].green);
            fprintf(output_image, "%d ", values[i][j].blue);
        }
        fprintf(output_image, "\n");
    }
    fclose(output_image);
    exit(0);
}

int main(int argc, char *argv[])
{
    //Creating two pipes for multi-directional communication
    int create_pipe[2];
    int data;
    data = pipe(create_pipe);
    if (data == -1)
    {
        perror("pipe");
    }
    int create_pipe2[2];
    int data2;
    data2 = pipe(create_pipe2);
    if (data2 == -1)
    {
        perror("pipe");
    }
          
    // int w, h, maxAscii;
    // char PPM_VERSION[10];
    // FILE *input_image = fopen(argv[1], "r");
    // fscanf(input_image, "%s%d%d%d", PPM_VERSION, &w, &h, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
    // vector<vector<one_pixel>> values(h, vector<one_pixel>(w)); //Vector for reading and storing pixels as a matrix. 
    // int red, green, blue;
    // for (int i = h - 1; i >= 0; i--)
    // {
    //     for (int j = 0; j <= w - 1; j++)
    //     {   //Storing RGB pixel values into above created matrix.
    //         fscanf(input_image, "%d%d%d", &red, &green, &blue);
    //         values[i][j].red = red;
    //         values[i][j].green = green;
    //         values[i][j].blue = blue;
    //     }
    // }
    // fclose(input_image);
    auto begin = chrono::high_resolution_clock::now(); // Starting the clock
    ConvertGrayscale(argv[1], create_pipe, create_pipe2, fork()); //T1 function on image
    HorizontalBlur(argv[2], create_pipe, create_pipe2, fork()); //T2 function on image

    wait(NULL);
    wait(NULL);

    auto end = chrono::high_resolution_clock::now(); //Stopping the clock
    auto taken_time = chrono::duration_cast<chrono::microseconds>(end - begin); // Calculating the time taken by T1 and T2.
    cout << "Time: " << taken_time.count() << " microseconds"<< endl;
    // FILE *output_image = fopen(argv[2], "w");
    // fprintf(output_image, "%s\n%d %d\n%d\n", PPM_VERSION, w, h, maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    // for (int i = h - 1; i >= 0; i--)
    // {
    //     for (int j = 0; j <= w - 1; j++)
    //     {   // Printing RGB pixel values from above updated image matrix.
    //         fprintf(output_image, "%d ", values[i][j].red);
    //         fprintf(output_image, "%d ", values[i][j].green);
    //         fprintf(output_image, "%d ", values[i][j].blue);
    //     }
    //     fprintf(output_image, "\n");
    // }
    // fclose(output_image);
}

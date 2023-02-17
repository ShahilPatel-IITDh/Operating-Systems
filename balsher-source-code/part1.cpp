#include <bits/stdc++.h>
#include <vector>
#include <chrono>

using namespace std;

//Creating struct structure for reading pixels easily in ppm3

struct one_pixel
{
    int red;
    int green;
    int blue;
};

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel values
void ConvertGrayscale(int w, int h, vector<vector<one_pixel>> &values)
{
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
}

//This function does a horizontal blur which makes a photo as if it is in motion.
void HorizontalBlur(int w, int h, vector<vector<one_pixel>> &values)
{
    int BLUR_AMOUNT = 50;
    for (int p = 0; p < h; p++)
    {
        for (int q = 0; q < w; q++)
        {
            int colour_red = values[p][q].red/2;
            int colour_green = values[p][q].green/2;
            int colour_blue = values[p][q].blue/2;
            if((w-q)<BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = w-q;
                for(int u = q+1; u < w; u++)
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
}

int main(int argc, char *argv[])
{
    int w, h, maxAscii;
    char PPM_VERSION[10];
    FILE *input_image = fopen(argv[1], "r");
    fscanf(input_image, "%s%d%d%d", PPM_VERSION, &w, &h, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
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
    auto begin = chrono::high_resolution_clock::now(); // Starting the clock
    ConvertGrayscale(w, h, values); //T1 function on image
    HorizontalBlur(w, h, values); //T2 function on image
    auto end = chrono::high_resolution_clock::now(); //Stopping the clock
    auto taken_time = chrono::duration_cast<chrono::microseconds>(end - begin); // Calculating the time taken by T1 and T2.
    cout << "Time: " << taken_time.count() << " microseconds"<< endl;
    FILE *output_image = fopen(argv[2], "w");
    fprintf(output_image, "%s\n%d %d\n%d\n", PPM_VERSION, w, h, maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    for (int i = h - 1; i >= 0; i--)
    {
        for (int j = 0; j <= w - 1; j++)
        {   // Printing RGB pixel values from above updated image matrix.
            fprintf(output_image, "%d ", values[i][j].red);
            fprintf(output_image, "%d ", values[i][j].green);
            fprintf(output_image, "%d ", values[i][j].blue);
        }
        fprintf(output_image, "\n");
    }
    fclose(output_image);
    return 0;
}

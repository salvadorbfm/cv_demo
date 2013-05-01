/*
      ### Computer Vision Demo ###

    - github.com/salvadorbfm/cv_demo
    - Author: Salvador Elizarrarás Montenegro
    - Licensed under the MIT license
*/
#ifndef _UI_HANDLER_H
#define _UI_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
using std::ifstream; // input file stream
using std::ios;
using std::ofstream;
namespace elements
{
    class UIHandler {
public:
    inline UIHandler()
    {
        this->first_time = true;
        this->next_folder = false;
        this->next_image = false;
        this->prev_image = false;
        this->is_auto = false;
        this->number_folder = 0;
        this->number_image = 0;
        this->max_number_of_images = 0;

        this->key = 'a';
        strcpy(this->format_image,"");
        //this->format_image

    }
    inline UIHandler(const UIHandler& orig)
    {

    }
    inline virtual ~UIHandler()
    {

    }
    //************ Properties
    bool first_time;
    bool next_image;
    bool prev_image;
    bool next_folder;
    bool is_auto;

    int number_image;
    int number_folder;
    int max_number_of_images;
    int algorithm;


    char key;
    char format_image[3];

    //************* Methods
    inline void parse_key(char c)
    {
        key = c;
        if((int)c > 47 && (int)c < 57 )    // Numbered Algorithms
            {
                algorithm = (int)c - 48;
                is_auto = false;

            }else if((int)c == 81 || (int)c == 97 || (int)c == -76 ) // Left Arrow
            {
                if(algorithm == 0)
                    algorithm=8;
                else
                    algorithm--;
                is_auto = false;
            }else if((int)c == 83 || (int)c == 100 || (int)c == -74)  //Right Arrow
            {
                if(algorithm == 8)
                    algorithm=0;
                else
                    algorithm++;
                is_auto = false;
            }else if((int)c == 82 || (int)c == 119 || (int)c == -72)
            {
                next_image = true;
            }else if((int)c == 84 || (int)c == 119 || (int)c == -78)
            {
                prev_image = true;
            }
            else if(c == 117)  //u,i,o,p
            {
                number_folder = 1;
            }else if(c == 105)
            {
                number_folder = 2;
            }else if(c == 111)
            {
                number_folder = 3;
            }else if(c == 112)
            {
                number_folder = 3;
            }else if(c == 9)
            {
                next_folder = true;
            }else if(c == 113 || c == 81) //Q o Q to AUTO MODE
            {
                is_auto = true;
            }
    }
    inline void parse_key_for_demo(char c)
    {
        key = c;
        if((int)c > 47 && (int)c < 57 )    // Numbered Algorithms
            {
                algorithm = (int)c - 48;
                is_auto = false;

            }else if((int)c == 81 || (int)c == 97 || (int)c == -76 ) // Left Arrow
            {
                if(algorithm == 0)
                    algorithm=9;
                else
                    algorithm--;
                is_auto = false;
            }else if((int)c == 83 || (int)c == 100 || (int)c == -74)  //Right Arrow
            {
                if(algorithm == 9)
                    algorithm=0;
                else
                    algorithm++;
                is_auto = false;
            }else if((int)c == 82 || (int)c == 119 || (int)c == -72)
            {
                if(algorithm == 9)
                    algorithm=0;
                else
                    algorithm++;
                is_auto = false;
            }else if((int)c == 84 || (int)c == 119 || (int)c == -78)
            {
                if(algorithm == 0)
                    algorithm=9;
                else
                    algorithm--;
                is_auto = false;
            }else if(c == 113 || c == 81) //Q o Q to AUTO MODE
            {
                is_auto = true;
            }
    }
    inline void select_folder()
    {
        if(next_folder == true)
        {
            if(number_folder == 5)
            {
                number_folder=0;
            }else
            {
                number_folder++;
            }
            first_time = true;
            number_image=0;
            next_folder = false;

        }
    }
    inline void select_image()
    {
        if(next_image==true)
        {
            if(number_image==max_number_of_images)
            {
                number_image = 0;
            }else
            {
                number_image++;
            }
            first_time = true;
        }else if(prev_image == true)
        {
            if(number_image == 0)
            {
                number_image = max_number_of_images;
            }else
            {
                number_image--;
            }
            first_time = true;
        }
        next_image = false;
        prev_image = false;
    }
    inline void load_configuration_file()
    {
        ifstream io;
        char configName[256]="";
        char buffer[8]="";
        sprintf(configName,"images/CC/TEST"
                        "/%d/folder.txt",number_folder);
        io.open(configName);
        if(io.is_open()==true)
        {
            io>> buffer;
            max_number_of_images = atoi(buffer);
            io>>format_image;
            io.close();
        }else
        {
            return ;
        }
    }
    inline void set_number_folder()
    {
        cout << endl << "Choose a folder number: " << endl;
        scanf("%d",&number_folder);
    }
    inline void set_number_folder(int number_folder)
    {
        this->number_folder = number_folder;
    }
    inline void set_number_image()
    {
        cout << endl << "Choose an image number: " << endl;
        scanf("%d",&number_image);
    }
    inline void set_number_image(int number_image)
    {
        this->number_image = number_image;
    }
    inline bool is_another_image_or_folder()
    {
        if(next_image==true || prev_image==true ||  next_folder == true)
        {
            return true;
        }else
        {
            return false;
        }
    }
    inline bool is_unable_to_continue()
    {
        if(key==120 || key==88 || next_image == true || prev_image ==  true || next_folder == true )//&& next_image == false && prev_image == false && next_folder == false)
        {
            return true;
        }else
        {
            return false;
        }
    }
    inline void set_algorithm(int algorithm)
    {
        // TODO Some validation
        this->algorithm = algorithm;
    }
private:

};
}
#endif
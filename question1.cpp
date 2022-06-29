// Any header you need here

// Any extra functions, declarations, etc. here

// This function reads a "Sensequake .bin" file located at [filepath], prints its header, and then prints the decoded data as text with a space between samples and a newline between frames.
//void print_sensequake_bin(const char* filepath)
//{
    // TODO: implement this
//}

// This code was written by Nandkshor Motiram Dhawale, On May 22nd 2022.
// This code is written to solve the assignment question no. 1
// The problem statement was given by the CTO member of https://www.sensequake.com/
// This code reads a "Setup7_sensor3.bin" file located at
// [C:\Projects\Solution_embedded_applicant_test_v2],
// prints its header, and then prints the decoded data as text with a
// pace between samples and a newline between frames.


#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
//
//void print_sensequake_bin(const char* filepath)
//{
    // TODO: implement this
//}


int buffToInteger(char * buffer) // This function is used to convert the decoded 4 bytes into an integer
{
    int a = int((unsigned char)(buffer[0]) << 24 |
            (unsigned char)(buffer[1]) << 16 |
            (unsigned char)(buffer[2]) << 8 |
            (unsigned char)(buffer[3]));
}

int main() {
char buffer[185]; // to read and store data from the .bin file
char frame1 [18];  //to store the frame1 data and to display it on the screen
char frame2 [18]; //to store the frame2 data and to display it on the screen
char frame3 [18]; //to store the frame3 data and to display it on the screen

fstream myFile("Setup7_sensor3.bin", ios::binary | ios::in |ios::out);
if(!myFile.is_open()) {
   cout << " error while opening the file" <<endl;
   } else {

myFile.read ((char *) &buffer,186);
    if (!myFile) {
        cout<<endl<<"The .bin file that you are searching is found and read." <<endl<<endl;
        cout<<endl<< "The total number of bytes read in the file were " << myFile.gcount() <<endl<<endl;
        //cout<< " " << buffer <<endl;

cout<<"Header 1 (approx. 32 Bytes)---> ";
    for(int i=0; i < 31; i++)
    {
            cout<<buffer[i];     // extracting the first header and displaying the data on the screen
    }

cout<<endl<<"Header 2 (approx. 32 Bytes)---> ";
    for(int i=34; i < 56; i++)
    {
        cout<<buffer[i];          // extracting the second header and displaying the data on the screen
    }

cout<<endl<<"Header 3 (approx. 18 Bytes)---> ";
    for(int i=57; i < 72; i++)
    {
        cout<<buffer[i];              // extracting the third header and displaying the data on the screen
    }

cout<<endl<<"Header 4 (approx.8 Bytes)---> ";
    for(int i=73; i < 81; i++)
    {
        cout<<buffer[i];              // extracting the fourth header and displaying the data on the screen
    }

cout<<endl<<"Header 5 (approx.13 Bytes)---> ";
    for(int i=84; i < 97; i++)
    {
        cout<<buffer[i];              // extracting the fifth header and displaying the data on the screen
    }

cout<<endl<<"Header 6 (approx. 23 Bytes)---> ";
    for(int i=99; i < 121; i++)
    {
        cout<<buffer[i];                  // extracting the sixth header and displaying the data on the screen
    }

cout<<endl<<endl<<"The size of Frame1 is 18 Bytes. ";
cout<<"The binary data is follows:  ";
    for(int i=131; i < 149; i++)
    {
        frame1[i-131]=buffer[i];
        cout<<frame1[i-131];             // extracting the sample data from first frame and displaying the data on the screen
    }

cout<<endl<<"The size of Frame2 is 18 Bytes. ";
cout<<"The binary data is as follows:  ";
    for(int i=150; i < 167; i++)
    {
        frame2[i-150]=buffer[i];
        cout<<frame2[i-150];            // extracting the sample data from second frame and displaying the data on the screen
    }
cout<<endl<<"The size of Frame3 is 18 Bytes ";
cout<<"The binary data is as follows:  ";
    for(int i=168; i < 185; i++)
    {
        frame3[i-168]=buffer[i];
        cout<<frame3[i-168];            // extracting the sample data from third frame and displaying the data on the screen
    }

}
}

myFile.clear();
myFile.close();

    int iposition;
    int oposition;

    ifstream infile;
    infile.open("Setup7_sensor3.bin", ios::binary | ios::in);  // open file again to extract the sample bytes and to decode
    ofstream ofile ("Decoded_Samples.bin",ios::binary |  ios::out); // write the decoded sample bytes into a file

    cout<<endl<<"The six variable axes & its samples are illustrated in a table shown below: "<<endl; // information
    cout<<endl<<endl<<"Vx"<<"\t     "<<"Vy"<<"\t     "<<"Vz"<<"\t     "<<"Ax"<<"\t     "<<"Ay"<<"\t     "<<"Az"<<endl<<endl;
    int k=131;
    int l=0;
    for (int j=0; j < 70; j=j+4)
    {
    infile.seekg(k, ios::beg);   //assign the seek cursor pointer to the fist before one of the starting sample
    iposition = infile.tellg();  //
    //cout<<"The first byte position of sample was:"<<iposition<< " ";
    char A[4];                     //create a char array to store the 4bytes
    infile.read ((char*)&A, sizeof(A));  //read the consecutive four bytes from the input file
    A[0]=0x0;                       // make the LSB byte to zero
    k=k+3;                          //increment the seek curs0r pointer to read the next 3 bytes

    ofile.seekp(j, ios::beg);
    oposition = ofile.tellp();
    //cout<<"Current Position of Opointer is:"<<oposition<<endl;
    ofile.write((char*)&A, sizeof(A)); // write the four decoded bytes to the output file

    int b=buffToInteger((char*)&A); //call function to convert the decoded bytes into integer value

        cout<<b<<" ";               // display the samples as per the axes variables
        if ((l == 5)|(l==11))
        {
         cout<<endl<<"<------   Newline between frames   -------->"<<endl;  // add newline between frames
        }
        l=l+1;
}
cout<<endl<<endl<<"There are a total 18 samples found as shown in 3 rows and 6 columns as above.,"; // Information
cout<<" The six in first row are from the frame 1.";                                                // Information
cout<<" The six in second row are from the frame 2.";                                               // Information
cout<<" The six in third row are from the frame 3.";                                                // Information
cout<<" Each sample in each frame row belongs to the six axes respectively.";                       // Information
cout<<" Therefore there are 3 samples per axes as shown in columns.";                               // Information
cout<<" The decoded binary file is also generated and saved in the folder.";                        // Information
cout<<" It can be viewed using HEX a viewer application such as Notepad++."<<endl;                  // Information

infile.close(); // close input file
ofile.close(); // close output file
}




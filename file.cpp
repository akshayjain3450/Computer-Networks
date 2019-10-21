#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    fstream file;
    int choice;
    string filename;
    while(1)
    {
        cout<<"Enter your choice"<<endl;
        cout<<"Enter 1 for creating file"<<endl;
        cout<<"Enter 2 for modifying file"<<endl;
        cout<<"Enter 3 for deleting file"<<endl;
        cin>>choice;
        switch(choice)
        {
            case 1:

                cout<<"Enter file name with extension:"<<endl;
                cin>>filename;
                file.open(filename, ios::out);
                if(!file)
                {
                    cout<<"Error in creating file!!!";
                }
                cout<<"file created successfully"<<endl;
                file.close();
                break;
            case 2:

                cout<<"Enter file name with extension:"<<endl;
                cin>>filename;
                file.open (filename, ios::out | ios::in );
                if(!file)
                {
                    cout<<"Error in creating file!!!";
                }
                cout<<"file modified successfully"<<endl;
                file.close();
                break;
            case 3:

                cout<<"Enter file name with extension:"<<endl;
                cin>>filename;
                if (remove(filename.c_str()) == 0)
                {
                    cout<<"Deleted successfully"<<endl;
                }
                else
                {
                    cout<<"Unable to delete the file"<<endl;
                }
                break;
            default:
                exit(1);
        }
    }


return 0;
}

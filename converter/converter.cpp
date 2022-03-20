#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream markdown, html;
    string checked_line, tags[2] = {"#"};
    int same_character_counter=0;

    markdown.open("index.md");
    html.open("index.html");
    
    while(!markdown.eof()) {
        getline(markdown, checked_line);

        for(int i=0; i<checked_line.length(); i++) {
            if(checked_line[i]=='#') same_character_counter++;
        }
        
        if(same_character_counter==1) {
            cout << "H1" << endl;
        }
            else if(same_character_counter==2) {
                cout << "H2" << endl;
            }
        else cout << "Something else" << endl;

        same_character_counter=0;
    }
    
    return(0);
}
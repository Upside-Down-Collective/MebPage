#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

string tag_recognition(string checked_line) {
    int same_character_counter=0;
    regex link("\\[(.*)\\]\\((.*)\\)");
    regex ordered_list("[0-9]+.(.*)");
    regex unordered_list("-(.*)|\\*(.*)|\\+(.*)");
    regex pre_opening("```opening");
    regex pre_closing("```");

    for(int i=0; i<checked_line.length(); i++) {
        if(checked_line[i]=='#') same_character_counter++;
    }

    if(same_character_counter==1) return("H1");
            else if(same_character_counter==2) return("H2");
            else if(same_character_counter==3) return("H3");
            else if(same_character_counter==4) return("H4");
            else if(same_character_counter==5) return("H5");
            else if(same_character_counter==6) return("H6");
            else if(regex_match(checked_line, link)) return("Link");
            else if(regex_match(checked_line, ordered_list)) return("Ordered list");
            else if(regex_match(checked_line, unordered_list)) return("Unordered list");
            else if(regex_match(checked_line, pre_opening)) return("Opening pre");
            else if(regex_match(checked_line, pre_closing)) return("Closing pre");
    else return("Unknown/Error");
}

int main() {
    ifstream markdown, html;
    string checked_line;

    markdown.open("index.md");
    html.open("index.html");
    
    while(!markdown.eof()) {
        getline(markdown, checked_line);

        cout << tag_recognition(checked_line) << endl;
    }
    
    return(0);
}
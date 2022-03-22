#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

// string to_italic(string s){
//     regex italic("(.*)\\*(.*)\\*(.*)");
//     s.find('str');
// }

string to_italic(string text) {
    int start = text.find("*");
    string first = text.substr(0, start);
    string second = text.substr(start);
    int end = second.find("*", 1);
    string italic = second.substr(1, end-1);
    string last = second.substr(end+1);

    return(first + " <span class=\"italic\"> " + italic + " </span> " + last);
}

string to_h1(string checked_line) {
    string text = checked_line.substr(2);
    string italic = to_italic(text);

    return("<h1> " + italic + " </h1>");
}

string tag_recognition(string checked_line) {
    int same_character_counter=0;
    regex link("\\[(.*)\\]\\((.*)\\)");
    regex ordered_list("[0-9]+.(.*)");
    regex unordered_list("-(.*)|\\*(.*)|\\+(.*)");
    regex pre_opening("```opening");
    regex pre_closing("```");
    regex italic("(.*)\\*(.*)\\*(.*)");
    string result;

    for(int i=0; i<checked_line.length(); i++) {
        if(checked_line[i]=='#') same_character_counter++;
    }

    if(same_character_counter==1) return to_h1(checked_line);
            // else if(regex_match(checked_line, italic)) tags+="italic ";
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
    return "";
}

int main() {
    fstream markdown, html;
    string checked_line, current_tag, text;

    markdown.open("index.md");
    html.open("index.html");
    
    while(!markdown.eof()) {
        getline(markdown, checked_line);
        current_tag = tag_recognition(checked_line);
        cout<<current_tag<<endl;
        text = "";
    }
    
    return(0);
}
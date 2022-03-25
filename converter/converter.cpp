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
    regex italic_regex("(.*)\\*(.*)\\*(.*)");

    int start_index = text.find("*");
    string first = text.substr(0, start_index);
    string italic_with_last = text.substr(start_index);
    int end_index = italic_with_last.find("*", 1);
    string italic = italic_with_last.substr(1, end_index-1);
    string last = italic_with_last.substr(end_index+1);

    string converted = first + "<span class=\"italic\">" + italic + "</span>" + last;

    if(regex_match(converted, italic_regex)) return(to_italic(converted));

    return(converted);
}

string to_h1(string checked_line) {
    regex italic_regex("(.*)\\*(.*)\\*(.*)");

    string text = checked_line.substr(2);
    string italic = text;

    if(regex_match(checked_line, italic_regex)){
      italic = to_italic(text);  
    } 

    return("<h1>" + italic + "</h1>");
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
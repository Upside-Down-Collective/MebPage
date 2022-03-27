#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

regex italic_regex("(.*)\\*(.*)\\*(.*)");
regex bold_regex("(.*)\\*\\*(.*)\\*\\*(.*)");
regex full_link_regex("(.*)\\[(.*)\\]\\((.*)\\)(.*)");
regex ordered_list_regex("[0-9]+.(.*)");
regex unordered_list_regex("-(.*)|\\*(.*)|\\+(.*)");
regex pre_opening_regex("```(.*)"); 
regex pre_closing_regex("```");

string to_italic(string checked_line) {
    regex full_italic_regex("(.*)\\*(.*)\\*(.*)");
    regex italic_regex("\\*(.*?)\\*");
    regex line_start_regex("(.*?)\\*");
    regex line_end_regex("\\*(.*)");

    string line_start = "";
    string line_end = "";

    smatch match;

    regex_search(checked_line, match, italic_regex);
    string italic = match.str(1);

    if (regex_search(checked_line, match, line_start_regex) == true) {
        line_start = match.str(1);
    }

    if (regex_search(checked_line, match, line_end_regex) == true) {
        line_end = match.str(1);
        line_end = line_end.substr(italic.length()+1);
    }

    string converted = line_start + "<span class=\"italic\">" + italic + "</span>" + line_end;
    if(regex_match(converted, full_italic_regex)) return(to_italic(converted));

    return(converted);
}

string to_bold(string checked_line) {
    regex bold_text_regex("\\*\\*(.*?)\\*\\*");
    regex line_start_regex("(.*?)\\*\\*");
    regex line_end_regex("\\*\\*(.*)");

    string line_start = "";
    string line_end = "";
    cout<<"checked: "<<checked_line<<endl;

    smatch match;

    regex_search(checked_line, match, bold_text_regex);
    string bold = match.str(1);

    if (regex_search(checked_line, match, line_start_regex) == true) {
        line_start = match.str(1);
    }

    if (regex_search(checked_line, match, line_end_regex) == true) {
        line_end = match.str(1);
        line_end = line_end.substr(bold.length()+2);
    }

    cout<<"bold text: "<<bold << endl;
    cout<<"line start: "<<line_start<<endl;
    cout<<"line end: "<<line_end<<endl;

    string converted = line_start + "<span class=\"bold\">" + bold + "</span>" + line_end;
    if(regex_match(converted, bold_regex)) return(to_bold(converted));

    return(converted);
}

string to_link(string checked_line){
    regex link_text_regex("\\[(.*?)\\]");
    regex link_regex("\\((.*?)\\)");
    regex line_start_regex("(.*?)\\[");
    regex line_end_regex("\\)(.*)");
    
    string line_start = "";
    string line_end = "";

    smatch match;

    regex_search(checked_line, match, link_text_regex);
    string link_text = match.str(1);

    regex_search(checked_line, match, link_regex);
    string link = match.str(1);

    if (regex_search(checked_line, match, line_start_regex) == true) {
        line_start = match.str(1);
    }

    if (regex_search(checked_line, match, line_end_regex) == true) {
        line_end = match.str(1);
    }
    
    string converted = line_start + "<a href=\"" + link + "\" target=\"!blank\">" + link_text + "</a>" + line_end;

    if(regex_match(converted, full_link_regex)) return(to_link(converted));

    return(converted);
}

string to_ordered_list(string checked_line) {
    regex number_length("([0-9]+.)");

    smatch match;

    regex_search(checked_line, match, number_length);
    string number = match.str(1);

    string text = checked_line.substr(number.length()+1);

    return("<li>" + text + "</li>");
}

string to_unordered_list(string checked_line) {
    string text = checked_line.substr(2);

    return("<li>" + text + "</li>");
}

string to_h(string checked_line, int n) {
    string text = checked_line.substr(n + 1);

    if(regex_match(checked_line, bold_regex)) {
        text = to_bold(text);
    }
    if(regex_match(checked_line, italic_regex)){
      text = to_italic(text);
    }

    if(regex_match(checked_line, full_link_regex)) {
        text = to_link(text);
    }

    char from_n =('0' + n);
    string n_to_string = "";
    n_to_string += from_n;


    return("<h" + n_to_string + ">" + text + "</h" + n_to_string+ ">");
}

string tag_recognition(string checked_line) {
    int same_character_counter=0;
    regex link("(.*)\\[(.*)\\]\\((.*)\\)(.*)");

    regex italic("(.*)\\*(.*)\\*(.*)");
    regex bold("(.*)\\*\\*(.*)\\*\\*(.*)");
    string result;

    for(int i=0; i<checked_line.length(); i++) {
        if(checked_line[i]=='#') same_character_counter++;
    }

    if(same_character_counter==1) return to_h(checked_line, 1);
            // else if(regex_match(checked_line, italic)) tags+="italic ";
            else if(same_character_counter==2) return to_h(checked_line, 2);
            else if(same_character_counter==3) return to_h(checked_line, 3);
            else if(same_character_counter==4) return to_h(checked_line, 4);
            else if(same_character_counter==5) return to_h(checked_line, 5);
            else if(same_character_counter==6) return to_h(checked_line, 6);
            else if(regex_match(checked_line, link)) return to_link(checked_line);
            else if(regex_match(checked_line, ordered_list_regex)) return to_ordered_list(checked_line);
            else if(regex_match(checked_line, unordered_list_regex)) return to_unordered_list(checked_line);
            else if(regex_match(checked_line, pre_opening_regex)) return("Opening pre");
            else if(regex_match(checked_line, pre_closing_regex)) return("Closing pre");
    else return("Unknown/Error");
    return "";
}

int main() {
    fstream markdown, html;
    string checked_line, current_tag, text;

    markdown.open("index.md");
    html.open("index.html");
    
    bool ol_start = false;
    bool ul_start = false;
    while(!markdown.eof()) {
        getline(markdown, checked_line);
        current_tag = tag_recognition(checked_line);

        if(!ol_start && regex_match(checked_line, ordered_list_regex)){
            ol_start = true;
            cout<< "<ol>";
        }
        else if(ol_start && !regex_match(checked_line, ordered_list_regex)){
            ol_start = false;
            cout<< "</ol>";
        }

        if(!ul_start && regex_match(checked_line, unordered_list_regex)){
            ul_start = true;
            cout<< "<ul>";
        }
        else if(ul_start && !regex_match(checked_line, unordered_list_regex)){
            ul_start = false;
            cout<< "</ul>";
        }
            
        cout<<current_tag<<endl;
        text = "";
    }

    if(ol_start){
        cout<<"</ol>";
    }

    if(ul_start) {
        cout << "</ul>";
    }
    
    return(0);
}
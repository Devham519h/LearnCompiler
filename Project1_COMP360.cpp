#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
using namespace std;
/// Devin Hamilton 
/// 10/03/26
/// COMP360 Project 1

// The types of Tokens used when analyzing the sample program
enum class TokType {keyw, ident, oper, scolon, 
    lbrace, rbrace, lparen, rparen, end_input, unknown};

class Token{
    public:
        string lexeme;
        TokType type;

    ///constructors
    Token(){this->type = TokType::unknown;}
    Token(string l, TokType type){this->lexeme = l; this->type = type;}
};

string describe_token(Token& token){
    if(token.type == TokType::end_input){return "end of input";}
    return "'" + token.lexeme + "'";
}
bool token_match(const vector<Token>& t_list, size_t& curr_pos, TokType type, const string& lexeme, const string& description){
    Token curr_token = t_list[curr_pos];
    if ((curr_token.type == type) && (lexeme.empty() || curr_token.lexeme == lexeme)){
        curr_pos++;
        return true;
    }
    cout << "Syntax error: expected " << description << ", found " << describe_token(curr_token) << endl;
    return false;
}

bool expr_parse(const vector<Token>& t_list, size_t& curr_pos){
    if(!token_match(t_list,curr_pos,TokType::ident,"", "identifier")){return false;}
    const Token& curr_token = t_list[curr_pos];
    if(curr_token.type == TokType::oper && (curr_token.lexeme == "*" || curr_token.lexeme == "/")){
        curr_pos++;
        return expr_parse(t_list, curr_pos);
    }
    return true;
}
bool assign_parse(const vector<Token>& t_list, size_t& curr_pos){
    if(!token_match(t_list,curr_pos,TokType::ident,"", "identifier")){return false;}
    else if(!token_match(t_list,curr_pos,TokType::oper,"=", "'='")){return false;}
    else if(!expr_parse(t_list,curr_pos)){return false;}
    else if(!token_match(t_list,curr_pos,TokType::scolon,"", "';'")){return false;}
    return true;
}

string type_str(TokType type){
    switch(type){
        case TokType::oper: return "operator";
        case TokType::keyw: return "keyword";
        case TokType::ident: return "identifier";
        case TokType::scolon: return "semicolon";
        case TokType::lbrace: return "left_brace";
        case TokType::rbrace: return "right_brace";
        case TokType::lparen: return "left_parentheses";
        case TokType::rparen: return "right_parentheses";
        case TokType::unknown: return "unknown";
        case TokType::end_input: return "EOF";
    }
    return "Check statement for any issues";
}
// This reads files and returns contents in string format
string read_file(string filename){

    std::ifstream file;
    string file_contents = "";
    string line;
    file.open(filename);
    if(!file.is_open()){
        cerr << "Error opening the file" << endl;
        return "";
    }
    else{while(getline(file,line)){file_contents += line + "\n";}}
    file.close();
    return file_contents;
}


//function to create token list for second half of program
vector<Token> create_token_list(string text){
    vector<Token> token_list;
    int i = 0;
    while(i < text.length()){
        string t(1, text.at(i));
        //check for whitespace
        if(std::isspace(static_cast<unsigned char>(text.at(i)))) {i++; continue;}

        //conditions to check to see the type of token
        if(std::isalpha(static_cast<unsigned char>(text.at(i)))) {
            int start_pos = i;
            TokType type;
            //put together a word
            while((i<text.length()) && (std::isalpha(static_cast<unsigned char>(text.at(i))))){i++;}
            string word = text.substr(start_pos, i-start_pos);
                
            // setting token type depending on the word
            if(word == "float"){type = TokType::keyw;} 
            else{type = TokType::ident;}
            token_list.push_back(Token(word,type));
        }
        else if(t == "("){token_list.push_back(Token(t,TokType::lparen)); i++;}
        else if(t == ")"){token_list.push_back(Token(t,TokType::rparen)); i++;} 
        else if(t == "{"){token_list.push_back(Token(t,TokType::lbrace)); i++;}
        else if(t == "}"){token_list.push_back(Token(t,TokType::rbrace)); i++;}
        else if(t == ";"){token_list.push_back(Token(t,TokType::scolon)); i++;}
        else if(t == "*" || t == "/" || t == "="){token_list.push_back(Token(t,TokType::oper)); i++;}
        else {token_list.push_back(Token(t,TokType::unknown)); i++;}
    }
    token_list.push_back(Token("",TokType::end_input));
    //return list
    return token_list;
}
int main(int argc, char* argv[]){
    string filename;
    string file_contents;
    vector<Token> token_list;

    // file can be input two different ways in this program since 
    // the program did not say which one:
    // command-line argument or user input (cin)
    if(argc >= 2){
        filename = string(argv[1]);
        file_contents = read_file(filename);
    }
    else{
        string input_file;
        getline(cin, input_file);
        file_contents = read_file(input_file);
    }
    if (file_contents == "") {return -1;}

    token_list = create_token_list(file_contents);

    for(const Token& token : token_list){
        cout << token.lexeme << " | " << type_str(token.type) << endl;
    }

    return 0;
}
//
// Created by Shrey Garg and Sai Deeduvanu on 4/4/23.
//
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include "compiler.h"
#include "lexer.h"
#include "algorithm"
#include <stack>
#include <iostream>

using namespace std;

LexicalAnalyzer lexer;
Token firstT;

bool checkToken(vector<string>, string);

struct InstructionNode * parse_generate_intermediate_representation()
{
    vector<string> indexArray;
    int countInstruction = 0;
    int count = 0;
    bool firstBrace = false;
    firstT = lexer.GetToken();
    stack<string > end;
    //if variable - set mem value to 0
    if(firstT.token_type == ID)
    {
        mem[next_available] = 0;
        indexArray.push_back(firstT.lexeme);
        next_available++;
        count++;
    }
    while(lexer.peek(count).token_type != END_OF_FILE)
    {
        Token token = lexer.peek(count);
        //if you have a character set mem value to 0 and proceed
        if(token.token_type == ID)
        {
            mem[next_available] = 0;
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it == indexArray.end())
            {
                indexArray.push_back(token.lexeme);
                next_available++;
            }
        }
            //else if add a constant value to mem
        else if(token.token_type == NUM)
        {
            mem[next_available] = stoi(token.lexeme);
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it == indexArray.end())
            {
                indexArray.push_back(token.lexeme);
                next_available++;
            }
        }

        else if (token.token_type == SEMICOLON)
        {
            countInstruction++;
        }

        else if(token.token_type == LBRACE)
        {
            firstBrace = true;
            end.push("{");
        }
        else if(token.token_type == RBRACE)
        {
            end.pop();
        }

        if(end.size() == 0 && firstBrace)
        {
            break;
        }
        count++;
    }

    count++;

    while(lexer.peek(count).token_type != END_OF_FILE)
    {
        Token token = lexer.peek(count);
        inputs.push_back(stoi(token.lexeme));
        count++;
    }

}
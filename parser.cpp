#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string>
#include <vector>
#include "compiler.h"
#include "lexer.h"
#include "algorithm"
#include <stack>
#include <iostream>

using namespace std;

LexicalAnalyzer lexer;
Token firstT;

struct InstructionNode * parse_generate_intermediate_representation()
{
    int cN = -1;
    vector<string> indexArray;
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
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it == indexArray.end())
            {
                mem[next_available] = 0;
                indexArray.push_back(token.lexeme);
                next_available++;
            }
        }
            //else if add a constant value to mem
        else if(token.token_type == NUM)
        {
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it == indexArray.end())
            {
                mem[next_available] = stoi(token.lexeme);
                indexArray.push_back(token.lexeme);
                next_available++;
            }
        }
        else if(token.token_type == LBRACE)
        {
            firstBrace = true;
            end.push("{");
        }
        else if(token.token_type == RBRACE)
        {
            cN++;
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
    count = 1, firstBrace = false;
    //Iterate through the program to create and assign the instruction nodes
    while(lexer.peek(count).token_type != END_OF_FILE)
    {
        Token token = lexer.peek(count);
        if(token.token_type == LBRACE)
        {
            firstBrace = true;
            end.push("{");
            count++;
            break;
        }
        else
        {
            count++;
        }
    }
    struct InstructionNode *  instruction = new InstructionNode;

    int checkFirstInstruction = 0;
    InstructionNode * noopArr[cN+1];
    int cNA = 0;
    while(!end.empty())
    {
        Token token = lexer.peek(count);
        struct InstructionNode *  temp = new InstructionNode;
        temp->next = NULL;
        if(token.token_type == INPUT)
        {
            temp->type = IN;
            token = lexer.peek(++count);
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it != indexArray.end())
            {
                temp->input_inst.var_index = it - indexArray.begin();
                if(checkFirstInstruction == 0)
                {
                    checkFirstInstruction++;
                    instruction = temp;
                }
                else
                {
                    struct InstructionNode *  temp1 = instruction;
                    while(temp1->next != NULL)
                    {
                        temp1 = temp1->next;
                    }
                    temp1->next = temp;
                }
                count = count + 2;
            }
        }
        else if(token.token_type == OUTPUT)
        {
            temp->type = OUT;
            token = lexer.peek(++count);
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it != indexArray.end())
            {
                temp->input_inst.var_index = it - indexArray.begin();
                struct InstructionNode *  temp1 = instruction;
                while(temp1->next != NULL)
                {
                    temp1 = temp1->next;
                }
                temp1->next = temp;
                count = count + 2;
            }
        }
        else if(token.token_type == ID)
        {
            temp->type = ASSIGN;
            token = lexer.peek(count);
            auto it0 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it0 != indexArray.end())
            {
                temp->assign_inst.left_hand_side_index = it0 - indexArray.begin();
                count = count + 2;
            }
            token = lexer.peek(count);
            if((token.token_type == NUM || token.token_type == ID) && (lexer.peek(count+1).token_type == SEMICOLON))
            {
                temp->assign_inst.op = OPERATOR_NONE;
                auto it1 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
                if(it1 != indexArray.end())
                {
                    temp->assign_inst.operand1_index = it1 - indexArray.begin();
                    if(checkFirstInstruction == 0)
                    {
                        checkFirstInstruction++;
                        instruction = temp;
                    }
                    else
                    {
                        struct InstructionNode *  temp1 = instruction;
                        while(temp1->next != NULL)
                        {
                            temp1 = temp1->next;
                        }
                        temp1->next = temp;
                    }
                    count = count + 2;
                }
            }
            else
            {
                auto it2 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
                if(it2 != indexArray.end())
                {
                    temp->assign_inst.operand1_index = it2-indexArray.begin();
                    count++;
                }
                token = lexer.peek(count);
                if(token.token_type == PLUS)
                {
                    temp->assign_inst.op = OPERATOR_PLUS;
                    count++;
                }
                else if(token.token_type == MINUS)
                {
                    temp->assign_inst.op = OPERATOR_MINUS;
                    count++;
                }
                else if(token.token_type == MULT)
                {
                    temp->assign_inst.op = OPERATOR_MULT;
                    count++;
                }
                else if(token.token_type == DIV)
                {
                    temp->assign_inst.op = OPERATOR_DIV;
                    count++;
                }
                token = lexer.peek(count);
                auto it3 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
                if(it3 != indexArray.end())
                {
                    temp->assign_inst.operand2_index = it3 - indexArray.begin();
                }
                if(checkFirstInstruction == 0)
                {
                    checkFirstInstruction++;
                    instruction = temp;
                }
                else
                {
                    struct InstructionNode *  temp1 = instruction;
                    while(temp1->next != NULL)
                    {
                        temp1 = temp1->next;
                    }
                    temp1->next = temp;
                }
                count = count + 2;
            }
        }
        else if(token.token_type == IF)
        {
            temp->type = CJMP;// if c <> a
            token = lexer.peek(++count);
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it != indexArray.end())
            {
                temp->cjmp_inst.operand1_index = it-indexArray.begin();
            }
            token = lexer.peek(++count);
            if(token.token_type == NOTEQUAL)
            {
                temp->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            }
            else if(token.token_type == GREATER)
            {
                temp->cjmp_inst.condition_op = CONDITION_GREATER;
            }
            else if(token.token_type == LESS)
            {
                temp->cjmp_inst.condition_op = CONDITION_LESS;
            }
            token = lexer.peek(++count);
            auto it1 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it1 != indexArray.end())
            {
                temp->cjmp_inst.operand2_index = it1-indexArray.begin();
            }
            struct InstructionNode * noop = new InstructionNode;
            noop->type = NOOP;
            noop->next = NULL;
            noopArr[cNA++] = noop;
            temp->cjmp_inst.target = noop;
            if(checkFirstInstruction == 0)
            {
                checkFirstInstruction++;
                instruction = temp;
            }
            else
            {
                struct InstructionNode *  temp1 = instruction;
                while(temp1->next != NULL)
                {
                    temp1 = temp1->next;
                }
                temp1->next = temp;
            }
            // if not (c <> a) skip forward to NOOP
            count = count + 1;
        }
        else if(token.token_type == WHILE)
        {
            temp->type = CJMP;
            count++;
            token = lexer.peek(count);
            auto it = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it != indexArray.end())
            {
                temp->cjmp_inst.operand1_index = it-indexArray.begin();
            }
            count++;
            token = lexer.peek(count);
            if(token.token_type == NOTEQUAL)
            {
                temp->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            }
            else if(token.token_type == GREATER)
            {
                temp->cjmp_inst.condition_op = CONDITION_GREATER;
            }
            else if(token.token_type == LESS)
            {
                temp->cjmp_inst.condition_op = CONDITION_LESS;
            }
            token = lexer.peek(++count);
            auto it1 = std::find(indexArray.begin(), indexArray.end(), token.lexeme);
            if(it1 != indexArray.end())
            {
                temp->cjmp_inst.operand2_index = it1-indexArray.begin();
            }
            struct InstructionNode * noop = new InstructionNode;
            noop->type = NOOP;
            noop->next = NULL;
            struct InstructionNode * jump = new InstructionNode;
            jump->type = JMP;
            jump->jmp_inst.target = temp;
            jump->next = noop;
            noopArr[cNA++] = jump;
            temp->cjmp_inst.target = noop;
            if(checkFirstInstruction == 0)
            {
                checkFirstInstruction++;
                instruction = temp;
            }
            else
            {
                struct InstructionNode *  temp1 = instruction;
                while(temp1->next != NULL)
                {
                    temp1 = temp1->next;
                }
                temp1->next = temp;
            }
            count++;
        }
        else if(token.token_type == FOR)
        {
            count++;
        }
        else if(token.token_type == SWITCH)
        {
            count++;
        }
        else if(token.token_type == LBRACE)
        {
            end.push("{");
            count++;
        }
        else if(token.token_type == RBRACE)
        {
            end.pop();
            if(!end.empty())
            {
                struct InstructionNode *  temp1 = instruction;
                while(temp1->next != NULL)
                {
                    temp1 = temp1->next;
                }
                temp1->next = noopArr[--cNA];
            }
            count++;
        }
        else
        {

        }
    }
    return instruction;
}
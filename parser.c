/**
 * Parser para transformar la entrada del shell
 * en tokens y devolverlos al shell para que los
 * procese
**/
#include "parser.h"
#include "shell_structures.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SPACE 0
#define QUOTES 1
#define ARG 2
#define OUTREDIR 3
#define INREDIR 4
#define AMPERSAND 5
#define APPEND 6
#define PIPE 7
#define COMMENT 8
#define CMD 9
#define FILL 10



int Parse(char * text, Command * outcmd[])
{
    text = removeSpaceAtBegining(text);
    if(text[0] == '#') return 0;
    text = removeSpaceAtEnd(text);
    text = removeOverSpace(text);


    int len = strlen(text);

    int * tokenizer = token_map(text);


    /*printf("%s\n", text);
    for (int i = 0; i < len; i++)
    {
        printf("%d", tokenizer[i]);
    }
    printf("\n");
    */



    int subcmdCount = 1;
    int cmdCount = 1;
    int background = 0;
    char *comment;
    int * tokenCounter = (int*)calloc(len, sizeof(int));

    //procesa el tokenCounter para saber cuantos caracteres seguidos tiene cada token,
    //busca los ampersand para dividir la linea en comandos
    //y pipes para divirla en subcomandos.
    //también guarda comenatrios en case de haber alguno.
    for (int i = 0; i < len; i++)
    {
        int counter = 0;
        int j = i;
        switch (tokenizer[i])
        {
        case PIPE:
            { 
                subcmdCount++;
            }
            break;
        case AMPERSAND:
            {
                background++; 
                cmdCount++;
            }
            break;
        case COMMENT:
            {
                comment = malloc(len - i);
                while (++i < len)
                {
                    ConcatChar(comment, text[i]);
                }
            }
            break;
        case SPACE:
            break;
        
        default:
        {
            int token = tokenizer[i];
            if (tokenizer[i] == APPEND)
            {
                i += 2;
            }
            while (j < len && tokenizer[j] == token)
            {
                tokenCounter[i]++;
                j++;
            }
            i = j - 1;
        }
        break;

        }
        
    }
    
    int currentCmdCount = 0;
    Command * currentCmd;
    subCommand * currentSubcmd;
    char * auxtext = calloc(len, sizeof(char));
    int i = 0;
    for (i = 0, currentCmd = initCommand(comment); i < len; i++)
    {
        currentSubcmd = initSubCommand("");

        while (i < len && tokenizer[i] != PIPE && tokenizer[i] != AMPERSAND)
        {
            switch (tokenizer[i])
                {
                case CMD:
                {
                    char* strCmd = calloc(tokenCounter[i], sizeof(char));
                    while (i < len && tokenizer[i] == CMD)
                    {
                        ConcatChar(strCmd, text[i]);
                        ConcatChar(auxtext, text[i]);
                        i++;
                    }
                    currentSubcmd->cmd = strCmd;
                    insertArg(currentSubcmd, strCmd);
                }
                    break;
                case ARG:
                {
                    char * arg = calloc(tokenCounter[i], sizeof(char));
                    while (i < len && tokenizer[i] == ARG)
                    {
                        ConcatChar(arg, text[i]);
                        ConcatChar(auxtext, text[i]);
                        i++;
                    }
                    insertArg(currentSubcmd, arg);
                }
                break;
                
                case INREDIR:
                {
                    ConcatChar(auxtext, text[i]);
                    i++;
                    char * inR = calloc(tokenCounter[i], sizeof(char));
                    if(i < len && tokenizer[i] == QUOTES)
                    {
                        free(inR);
                        inR = calloc(tokenCounter[i], sizeof(char));
                        inR = getQuotes(text, i, tokenCounter[i]);
                        ConcatChar(auxtext, '"');
                        strcat(auxtext, inR);
                        ConcatChar(auxtext, '"');
                        i+=tokenCounter[i];
                    }
                    else
                    {
                        while (i < len && tokenizer[i] == INREDIR)
                        {
                            ConcatChar(inR, text[i]);
                            ConcatChar(auxtext, text[i]);
                            i++;
                        }
                    }
                    insertInRedir(currentSubcmd, inR);
                }
                break;

                case OUTREDIR:
                {
                    ConcatChar(auxtext, text[i]);
                    i++;
                    char * outR = calloc(tokenCounter[i], sizeof(char));
                    if(i < len && tokenizer[i] == QUOTES)
                    {
                        free(outR);
                        outR = calloc(tokenCounter[i], sizeof(char));
                        outR = getQuotes(text, i, tokenCounter[i]);
                        ConcatChar(auxtext, '"');
                        strcat(auxtext, outR);
                        ConcatChar(auxtext, '"');
                        i+=tokenCounter[i];
                    }
                    else
                    {
                        while (i < len && tokenizer[i] == OUTREDIR)
                        {
                            ConcatChar(outR, text[i]);
                            ConcatChar(auxtext, text[i]);
                            i++;
                        }
                    }
                    insertOutRedir(currentSubcmd, outR);
                }
                break;

                case APPEND:
                {
                    i+=2;
                    strcat(auxtext, ">>");
                    char * appR = calloc(tokenCounter[i], sizeof(char));

                    if(i < len && tokenizer[i] == QUOTES)
                    {
                        free(appR);
                        appR = calloc(tokenCounter[i], sizeof(char));
                        appR = getQuotes(text, i, tokenCounter[i]);
                        ConcatChar(auxtext, '"');
                        strcat(auxtext, appR);
                        ConcatChar(auxtext, '"');
                        i+=tokenCounter[i];
                    }
                    else
                    {
                        while (i < len && tokenizer[i] == APPEND)
                        {
                            ConcatChar(appR, text[i]);
                            ConcatChar(auxtext, text[i]);
                            i++;
                        }
                    }
                    insertAppendRedir(currentSubcmd, appR);
                }
                break;

                case QUOTES:
                {
                    char * arg = calloc(tokenCounter[i], sizeof(char));
                    arg = getQuotes(text, i, tokenCounter[i]);
                    ConcatChar(auxtext, '"');
                    strcat(auxtext, arg);
                    ConcatChar(auxtext, '"');
                    i+=tokenCounter[i];
                    insertArg(currentSubcmd, arg);
                }
                break;
                default:
                {
                    ConcatChar(auxtext, text[i]);
                    i++;
                }
                    break;
                }
            
        }
        
        insertSubcommand(currentCmd, currentSubcmd);

        if(i < len && tokenizer[i] == AMPERSAND)
        {
            currentCmd->_background = 1;
            currentCmd->cmdtext = auxtext;
            if(i <len - 1)
            {
                outcmd[currentCmdCount] = currentCmd;
                currentCmdCount++;
                currentCmd = initCommand(comment);
                auxtext = calloc(len, sizeof(char));
            }
        }
        
    }

    currentCmd->cmdtext = auxtext;
    outcmd[currentCmdCount] = currentCmd;
    currentCmdCount++;
    outcmd[currentCmdCount] = NULL;

    free(tokenCounter);
    free(tokenizer);
    return currentCmdCount;
}


//busca si existe algun signo
//ampersand en el comando
int Background(char * text)
{
    int count = strlen(text);
    for(int i = count; i >=0; i--)
    {
        if(text[i] == '&')
        {
            return 1;
        }
    }

    return 0;
}


char * removeOverSpace(char * line)
{
    int len = strlen(line);
    if(len == 0)
    {
        return "";
    }
    int wasSpace = 0;
    char * aux = malloc(len);
    int i;
    int j = 0;
    char lastChar = '[';
    int insideQutes = 0;
    for(i = 0; i < len ; i++)
    {
        if(line[i] == ' ')
        {
            if(IsOperator(lastChar) && !insideQutes){ continue;}
            if(wasSpace && !insideQutes)
            {
                continue;
            }
            else
            {
                wasSpace = 1;
            }
        }
        else
        {
            lastChar = line[i];
            if(lastChar == '"'){ insideQutes = !insideQutes;}
            wasSpace = 0;
        }

        aux[j] = line[i];
        j++;
    }
    aux[j] = '\0';

    line = aux;

    return aux;
}

char * removeSpaceAtBegining(char * line)
{
    int len = strlen(line);
    char * aux = calloc(len+1, sizeof(char));
    int j = 0;
    int i = 0;
    while (i < len && line[i] == ' ')
    {
        i++;
    }
    for (; i < len; i++, j++)
    {
        aux[j] = line[i];
    }
    return aux;
    
}

char * removeSpaceAtEnd(char * line)
{
    int len = strlen(line);
    char * aux = calloc(len+1, sizeof(char));
    int j = 0;
    int i = len - 1;
    while (i > 0 && line[i] == ' ')
    {
        i--;
    }
    for (j = i; i >= 0 ; i--, j--)
    {
        aux[j] = line[i];
    }
    return aux;
    
}


int * token_map(char * text)
{
    int len = len;
    int cmd = 1;
    int * tokenizer = (int*)malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++)
    {
        if(cmd){tokenizer[i] = CMD; cmd = 0; continue; };
        char cmp = text[i];
        if(cmp == ' '){ tokenizer[i] = SPACE; }
        else if(cmp == '|'){ tokenizer[i] = PIPE; cmd = 1; }
        else if(cmp == '&'){ tokenizer[i] = AMPERSAND; cmd = 1;}
        else if(cmp == '<'){ tokenizer[i] = INREDIR;cmd = 0;}
        else if(cmp == '>'){ tokenizer[i] = OUTREDIR; cmd = 0;}
        else if(cmp == '#'){ tokenizer[i] = COMMENT; cmd = 0;}
        else if(cmp == '"'){ tokenizer[i] = QUOTES; }
        else { tokenizer[i] = FILL; cmd = 0;}
    }
    
    for (int i = 0; i < len; i++)
    {
        switch (tokenizer[i])
        {
            case QUOTES:
            {
                i++;
                while (i < len && tokenizer[i] != QUOTES)
                {
                    tokenizer[i] = QUOTES;
                    i++;
                }
                
            }
                break;
            
            case OUTREDIR:
            {
                int redir = OUTREDIR;
                if(i < len - 1 && tokenizer[i+1] == OUTREDIR)
                {
                    tokenizer[i] = APPEND;
                    i++;
                    tokenizer[i] = APPEND;
                    redir = APPEND;
                }

                i++;
                while (i < len && (tokenizer[i] == FILL || tokenizer[i] == SPACE))
                {
                    if(tokenizer[i] == SPACE)
                    {
                        i++;
                        continue;
                    }
                    tokenizer[i] = redir;
                    i++;
                }

                i--;
            }
                break;

            case INREDIR:
            {
                i++;
                while (i < len && (tokenizer[i] == SPACE || tokenizer[i] == FILL))
                {
                    if(tokenizer[i] == SPACE){ i++; continue;}
                    tokenizer[i] = INREDIR;
                }
                i--;            
            }
                break;

            case CMD:
            {
                i++;
                while (i < len && tokenizer[i] == FILL)
                {
                    tokenizer[i] = CMD;
                    i++;
                }

                i--;
            }
                break;

            case COMMENT:
            {
                i++;
                while (i < len)
                {
                    tokenizer[i] = COMMENT;
                    i++;
                }
            }
                break;

            case FILL:
            {
                while (tokenizer[i] == FILL)
                {
                    tokenizer[i] = ARG;
                    i++;
                }
                i--;
            }
                break;
            default:
                break;
        }
        
    }

    
    return tokenizer;
}

int IsOperator(char character)
{
    return (character == '<' || character == '>' || character == '&' || character == '|');
}

void ConcatChar(char * str1, char _char)
{
    char aux[] = " \0";
    aux[0] = _char;
    strcat(str1, aux);
}

char * getQuotes(char * buff, int index, int size)
{
    char * out = calloc(size, sizeof(char));
    for (int i = index + 1; i < index + size - 1; i++)
    {
        ConcatChar(out, buff[i]);
    }
    
    return out;
}
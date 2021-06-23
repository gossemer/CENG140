#include <stdio.h>
#include <stdlib.h>


void pushStack(char,char*,int,int*);
char popStack(char*,int,int*);
void enqueue(char,char*,int);

void get_input(char*,int*,int*);

void reversePolish(char*,char*,int,int);

char *basic_derivative(char*);
void infix(char *,char *,int *,int,int);
void operand_determine(int,int,char*,int*,int*);
void splitter(char*,int*,int);
char *derive(char *,int *,int,int,int);


int main()
{
    char *expression,*postfix;
    int len,op_count,i;
    int *split;
    expression = calloc(20,sizeof(char));
    get_input(expression,&len,&op_count);
    postfix = calloc(len,sizeof(char));
    reversePolish(expression,postfix,len,op_count);
    splitter(postfix,split,len);
    for(i =0;split[i];i++);
    printf("%s",derive(postfix,split,0,split[i]+1,op_count));
    return 0;
}

/* STACK FUNCTIONS */

void pushStack(char element,char *stack, int stack_size, int *top_index)
{
    if(*top_index == -1)/*stack is empty*/
    {
        *top_index = stack_size-1;
        stack[*top_index] =  element;
    }
    else if(*top_index == 0)/*stack is full*/
    {/*does nothing because there will be no push if stack is full*/
    }
    else
    {
        stack[--(*top_index)] = element;
    }

}

char popStack(char *stack, int stack_size, int *top_index)
{
    char popped_item;
    if(*top_index == -1)/*stack is empty*/
    {
        /*does nothing*/
    }
    else
    {
        popped_item = stack[*top_index];
        if(*top_index == stack_size-1)
        {
            *top_index =-1;
        }
        else
        {
            (*top_index)++;
        }
    }

    return popped_item;
}

/*QUEUE PUSH FUNCTION*/
void enqueue(char element,char *queue,int l_index)
{
    queue[l_index] = element;
}

void reversePolish(char *expression,char *postfix,int len,int op_count)
{
    int top_index,i,l_index;
    char op_stack[op_count];
    top_index = -1;
    l_index =0;
    for(i =0; i<len;i++)
    {
        char c;
        c = expression[i];
        switch(c)
        {
            case '(':
                pushStack(c,op_stack,op_count,&top_index);
                break;
            case ')':
                while(op_stack[top_index] != '(')
                {
                    enqueue(popStack(op_stack,op_count,&top_index),postfix,l_index++);
                }
                popStack(op_stack,op_count,&top_index);
                break;

            case '+':
            case '-':
                if(top_index ==-1)
                {
                    pushStack(c,op_stack,op_count,&top_index);
                }
                else if(op_stack[top_index] == '(')
                {
                    pushStack(c,op_stack,op_count,&top_index);
                }
                else
                {
                    while(top_index!=-1)
                    {
                        if(op_stack[top_index] == '(')
                        {
                            break;
                        }
                        else
                        {
                            enqueue(popStack(op_stack,op_count,&top_index),postfix,l_index++);
                        }
                    }
                    pushStack(c,op_stack,op_count,&top_index);
                }
                break;

            case '*':
            case '/':
                if(top_index ==-1)
                {
                    pushStack(c,op_stack,op_count,&top_index);
                }
                else if(op_stack[top_index] == '(' ||op_stack[top_index] == '-' ||op_stack[top_index] == '+')
                {
                    pushStack(c,op_stack,op_count,&top_index);
                }
                else
                {
                    while(top_index!=-1)
                    {
                        if(op_stack[top_index] == '(' ||op_stack[top_index] == '-' ||op_stack[top_index] == '+')
                        {
                            break;
                        }
                        else
                        {
                            enqueue(popStack(op_stack,op_count,&top_index),postfix,l_index++);
                        }
                    }
                    pushStack(c,op_stack,op_count,&top_index);
                }                
                break;
            case '^':
                pushStack(c,op_stack,op_count,&top_index);
                break;
            default:
                enqueue(c,postfix,l_index++);

        }
    }
    while(top_index != -1)
    {
        enqueue(popStack(op_stack,op_count,&top_index),postfix,l_index++);
    }

}


void get_input(char *expression,int *len,int *op_count)
{
    char c;
    *op_count = 0;
    for(*len = 0,c = getchar(); c!= EOF; (*len)++)
    {
        if(c== 32 || c == 10) 
        {
            (*len)--;
        }
        else
        {
            if(*len < 20)
            {
                expression[*len] = c;
            }
            else
            {
                expression = realloc(expression,*(len)+1);
                expression[*len] = c;
            }
                
        }
        if(c == '+' || c == '-' || c == '^' || c == '/' || c == '*' || c == '(') (*op_count)++;
        c = getchar();
        
    }
}

void splitter(char *rpn,int *split,int len)
{ 
    int i,j;
    for(i =0,j=0; i<len;i++)
    {
        char c;
        int k;
        c = rpn[i];
        switch(c)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case 'X':
                split[j++] = i;
                break;

            case 's':
                if(rpn[i+1] == 'h')
                {
                    split[j++] = i;
                    i+=1;
                }
                else
                {
                    split[j++] = i;
                    i+=2;
                } 
                break;

            case 'c':
                split[j++] = i;
                i+=1;
                break;
            
            case 't':
                split[j++] = i;
                i+=2;
                break;
            
            case 'l':
                split[j++] = i;
                i+=1;
                break;
            
            default:
                
                for(k =0; rpn[k+i]<58 && rpn[k+i]>47;k++);
                split[j++] = i;
                i +=k-1;

            
        }
    }
}
void operand_determine(int start, int end,char *postfix,int *op2,int *split)
{
    int k,i;
    for(k =1,i =end-1;k!= 0;i--)
    {
        
        char c;
        c = postfix[split[i]];
        if(c ==  '+' || c ==  '-' || c ==  '*' || c ==  '/' || c ==  '^' )
        {
            k++;
        }
        else
        {
            k--;
        }
    }
    *op2 = i+1;

}

char *basic_derivative(char *func)
{
    switch(func[0])
    {
        case 'X':
            return "(1)";
        case 's':
            if(func[1] == "(h)")
            {
                return "(ch)";
            }
            else return "(cos)";
        case 'c':
            return "(sh)";
        case 'l':
            return "(1/X)";
        case 't':
            return "(sec^2)";
    }
}


/*TODO: POSTFIX TO INFIX FUNCTION*/

void infix(char*infix,char *postfix,int *split,int len,int op_count)
{   
    int i;
    char operand;
    infix = calloc(len+2*op_count,sizeof(char));
    for(i = 0; i<sizeof(split); i++)
    {
        char c;
        c = postfix[split[i]];
        if(c ==  '+' || c ==  '-' || c ==  '*' || c ==  '/' || c ==  '^')
    }
}


char *derive(char *postfix,int *split,int start,int end,int op_count)
{
    int op1,op2,i,len;
    char operator;
    char operand1[3];
    char operand2[3];
    char new_exp[2];
    char *func,*infix, *derivative[14];
    int func_len = split[end]-split[start];
    func = calloc(func_len,sizeof(char));
    for(i=start; i<end;i++)
    {
        func[i-start] = postfix[i];
    }
    operator = postfix[split[end]];
    operand_determine(start,end,postfix,&op2,split);
    switch(operator)
    {
        case '+':
        case '-':
            derivative[0] = derive(postfix,split,start,op2-1,op_count-1);
            derivative[1] = operator;
            derivative[2] = derive(postfix,split,op2,end,op_count-1);
            return derivative;
        case '*':
            derivative[0] = "(";
            derivative[1] = derive(postfix,split,start,op2-1,op_count-1);
            derivative[2] = "*";
            derivative[3] = infix(operand1); 
            derivative[4] = "+";
            derivative[5] = infix(operand2);
            derivative[6] = "*";
            derivative[7] = derive(postfix,split,op2,end,op_count-1);
            derivative[8] = ")";
            return derivative;
        case '/':
            derivative[0] = "(";
            derivative[1] = derive(postfix,split,start,op2-1,op_count-1);
            derivative[2] = "*";
            derivative[3] = infix(operand1); 
            derivative[5] = infix(operand2);
            derivative[6] = "*";
            derivative[7] = derive(postfix,split,op2,end,op_count-1);
            derivative[8] = ")";
            derivative[9] = "/";
            derivative[10] = "(";
            derivative[11] = infix(operand2);
            derivative[12] = "^2";
            derivative[13] = ")";
            return derivative;
        case '^':
            for(i= start;i<op2;i++)
            {
                operand1[i-start] = postfix[split[i]];
            }
            for(i = op2; i<end;i++)
            {
                operand2[i-op2] = postfix[split[i]];
            }
            len = i-op2+1;
            if(len == 1)
            {
                new_exp[0] = operand2[0]-1;
            }
            else if(len == 2)
            {
                if(operand2[1] == '0')
                {
                    if(operand2[0] == '1')
                    {
                        new_exp[0] = '9';
                    }
                    else
                    {
                        new_exp[0] = operand2[0]-1;
                        new_exp[1] = '9'; 
                    }
                }
                else
                {
                    new_exp[0] = operand2[0];
                    new_exp[1] = operand2[1] -1;
                }
            }
            else
            {
                new_exp[0] = '9';
                new_exp[1] = '9';
            }
            derivative[0] = "(";
            derivative[1] = operand2;
            derivative[2] = "*";
            derivative[3] = infix(operand1);
            derivative[4] = "^";
            derivative[5] = new_exp;
            derivative[6] = "*";
            derivative[7] = derive(postfix,split,start,op2-1,op_count-1);
            derivative[8] = ")";
            return derivative;
        default:
            return basic_derivative(func);

    }
}
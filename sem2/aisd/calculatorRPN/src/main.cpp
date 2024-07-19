#include <iostream>

using namespace std;

enum data {
    ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION,
    NUMBER, OPEN_BRACKET, CLOSE_BRACKET,
    IF, MAX, MIN, COMMA, N
};

struct Stack
{
    int info;
    int value;
    Stack* next = NULL;
};

// function to add element to stack
void push_back(Stack** p, int x, int value = 0)
{
    Stack* t = new Stack;
    t->info = x;
    t->value = value;

    if (*p == NULL)
    {
        *p = t;
    
}
    else
    {
        Stack* last = *p;
        while (last->next != NULL)
        {
            last = last->next;
        
}

        last->next = t;
    
}
}

// function to get top element of stack
int top(Stack* p, bool value = false)
{
    if (p != NULL)
    {
        while (p->next != NULL)
        p = p->next;
        if (value)
            return p->value;
        return p->info;
    
}   
    return 0;
}

// function to remove element from stack
void pop_back(Stack** p)
{
    if (*p == NULL)
    {
        return;
    
}
    if ((*p)->next == NULL)
    {
        delete *p;
        *p = NULL;
        return;
    
}
    Stack* t = *p;
    while (t->next->next != NULL)
    {
        t = t->next;
    
}
    delete t->next;
    t->next = NULL;
}



// function to view stack
void view(Stack* p, bool reversed = false)
{
    const char x[][20] = { "+", "-", "*", "/", "NUMBER", "(", ")", "IF", "MAX", "MIN", ",", "N" 
};
    if (reversed)
    {
        Stack* t = p;
        if (t->next != NULL)
        {
            view(t->next, 1);
        
}

        if (t->info == NUMBER)
            cout << t->value << " ";
        else if (t->info == MIN && t->value > 0)
            cout << x[t->info] << t->value << " ";
        else if (t->info == MAX && t->value > 0)
            cout << x[t->info] << t->value << " ";
        else if (t->info == IF)
            cout << x[t->info] << " ";
        else
            cout << x[t->info] << " ";
        
    
} else {
        while (p != NULL)
        {
            if (p->info == NUMBER)
                cout << p->value << "  ";
            else if (p->info == MIN && p->value > 0)
                cout << x[p->info] << p->value << "  ";
            else if (p->info == MAX && p->value > 0)
                cout << x[p->info] << p->value << "  ";
            else if (p->info == IF)
                cout << x[p->info] << "  ";
            else
                cout << x[p->info] << "  ";
            p = p->next;
        
}
    
}
}

// function to delete stack
void deleteStack(Stack** p) {
    while (*p != NULL) {
        Stack* next = (*p)->next;
        delete *p;
        *p = next;
    
}
}
// function to get priority of operator
int priority(int x)
{
    if (x == ADDITION || x == SUBTRACTION)
        return 1;
    if (x == MULTIPLICATION || x == DIVISION)
        return 2;
    if (x == N || x == MIN || x == MAX || x == IF || x == OPEN_BRACKET || x == CLOSE_BRACKET || x == COMMA)
        return 3;
    return 0;
}

// RPN calculation
void calculate(Stack** p)
{
    Stack* stack = NULL;
    while (*p != NULL)
    {
        if ((*p)->info == NUMBER)
        {
            push_back(&stack, (*p)->info, (*p)->value);
        
}
        else if ((*p)->info == N)
        {
            cout << "N" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            push_back(&stack, NUMBER, -a);
        
}
        else if ((*p)->info == ADDITION)
        {
            cout << "+" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            int b = top(stack, 1);
            pop_back(&stack);
            push_back(&stack, NUMBER, a + b);
        
}
        else if ((*p)->info == SUBTRACTION)
        {
            cout << "-" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            int b = top(stack, 1);
            pop_back(&stack);
            push_back(&stack, NUMBER, b - a);
        
}
        else if ((*p)->info == MULTIPLICATION)
        {
            cout << "*" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            int b = top(stack, 1);
            pop_back(&stack);
            push_back(&stack, NUMBER, a * b);
        
}
        else if ((*p)->info == DIVISION)
        {
            cout << "/" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            int b = top(stack, 1);
            pop_back(&stack);
            if (a == 0){
                cout << "ERROR" << endl;
                deleteStack(&stack);
                return;
            
}
            push_back(&stack, NUMBER, b / a);
        
}
        else if ((*p)->info == MIN)
        {
            cout << "MIN" << (*p)->value << " ";
            view(stack, 1);
            cout << endl;
            int minValue = top(stack, 1);
            int n = (*p)->value;
            pop_back(&stack);
            while (n > 1)
            {
                int a = top(stack, 1);
                pop_back(&stack);
                if (a < minValue)
                    minValue = a;
                n--;
            
}
            push_back(&stack, NUMBER, minValue);
        
}
        else if ((*p)->info == MAX)
        {
            cout << "MAX" << (*p)->value << " ";
            view(stack, 1);
            cout << endl;
            int maxValue = top(stack, 1);
            int n = (*p)->value;
            pop_back(&stack);
            while (n > 1)
            {
                int a = top(stack, 1);
                pop_back(&stack);
                if (a > maxValue)
                    maxValue = a;
                n--;
            
}
            push_back(&stack, NUMBER, maxValue);
        
}
        else if ((*p)->info == IF)
        {
            cout << "IF" << " ";
            view(stack, 1);
            cout << endl;
            int a = top(stack, 1);
            pop_back(&stack);
            int b = top(stack, 1);
            pop_back(&stack);
            int c = top(stack, 1);
            pop_back(&stack);
            push_back(&stack, NUMBER, c > 0 ? b : a);
        
}
        Stack* t = *p;
        (*p) = (*p)->next;
        delete t;

    
}
    cout << top(stack, 1) << endl;
    deleteStack(&stack);
    
}

// function to convert infix to RPN
int toRPN(Stack** p, Stack** stack, Stack** result)
{
    int counter = 1;
    int nCounter = 0;

    while (*p != NULL)
    {
        if ((*p)->info == NUMBER)
        {
            push_back(result, (*p)->info, (*p)->value);
            while (nCounter > 0)
            {
                push_back(result, N);
                nCounter--;
            
}
        
}
        else if ((*p)->info == MIN || (*p)->info == MAX || (*p)->info == IF || (*p)->info == OPEN_BRACKET)
        {
            push_back(stack, (*p)->info);
            (*p) = (*p)->next;
            toRPN(p, stack, result);
            while (nCounter > 0)
            {
                push_back(result, N);
                nCounter--;
            
}
        
}
        else if ((*p)->info == CLOSE_BRACKET)
        {
            while (top(*stack) != OPEN_BRACKET && top(*stack) != MIN && top(*stack) != MAX && top(*stack) != IF)
            {
                if (top(*stack) != COMMA)
                    push_back(result, top(*stack));
                pop_back(stack);
            
}
            if (top(*stack) != OPEN_BRACKET && top(*stack) != COMMA)
            {
                push_back(result, top(*stack), counter);
                
            
}
            pop_back(stack);
            return counter;
            
        
} else if ((*p)->info == N)
        {
            nCounter++;
        
}
        else if ((*p)->info == COMMA)
        {
            
            counter++;
            while (top(*stack) != OPEN_BRACKET && top(*stack) != MIN && top(*stack) != MAX && top(*stack) != IF && top(*stack) != COMMA)
            {
                push_back(result, top(*stack));
                pop_back(stack);
            
}
            push_back(stack, COMMA);
        
}
        else
        {
            int priority_p = priority((*p)->info);
            while (*stack != NULL && priority(top(*stack)) >= priority_p && top(*stack) != OPEN_BRACKET && top(*stack) != MIN && top(*stack) != MAX && top(*stack) != IF)
            {
                if (top(*stack) != COMMA)
                    push_back(result, top(*stack));
                pop_back(stack);
            
}
            push_back(stack, (*p)->info);
        
}
        (*p) = (*p)->next;
    
}
    return counter;
}

void toRPNWrapper(Stack* p)
{
    Stack* stack = NULL;
    Stack* result = NULL;
    toRPN(&p, &stack, &result);
    
    while (stack != NULL)
    {
        push_back(&result, top(stack));
        pop_back(&stack);
    
}
    view(result);
    cout << endl;
    
    calculate(&result);
    deleteStack(&result);
    deleteStack(&stack);
    
}

void handler(){
    Stack* t = NULL;
    char c;

    while (cin >> c && c != '.')
    {
        if (isdigit(c))
        {
            int number = c - '0';
            while (isdigit(cin.peek()))
            {
                cin >> c;
                number = number * 10 + (c - '0');
            
}
            push_back(&t, NUMBER, number);
        
}
        else if (c == '+')
        {
            push_back(&t, ADDITION);
        
}
        else if (c == '-')
        {
            push_back(&t, SUBTRACTION);
        
}
        else if (c == '*')
        {
            push_back(&t, MULTIPLICATION);
        
}
        else if (c == '/')
        {
            push_back(&t, DIVISION);
        
}
        else if (c == '(')
        {
            push_back(&t, OPEN_BRACKET);
        
}
        else if (c == ')')
        {
            push_back(&t, CLOSE_BRACKET);
        
}
        else if (c == 'I')
        {
            push_back(&t, IF);
            cin.ignore(3);
        
}
        else if (c == 'M')
        {
            cin >> c;
            if (c == 'A')
            {
                push_back(&t, MAX);
                cin.ignore(3);
            
}
            else if (c == 'I')
            {
                push_back(&t, MIN);
                cin.ignore(3);
            
}
        
}
        else if (c == 'N')
        {
            push_back(&t, N);
        
}
        else if (c == ',')
        {
            push_back(&t, COMMA);
        
}
    
}
    toRPNWrapper(t);
    deleteStack(&t);
}

int main()
{
    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        handler();
        cout << endl;
    
}
}
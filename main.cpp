#include <iostream>
#include <string>


// code definition reading/returning a character to/from a stream.
#define nextSymbol ch=text[ind++];
#define backSymbol if((--ind)<0)ind=0;
// Interrupt the execution of the function on the set error flag.
#define errorExit if(stateErr)return 0;

// type of token
enum type_token { NONE, NUMBER, OPERATION, END };
// subtype of token for OPERATION
enum operation { PLUS, MINUS, MULTIPLICATION, DIVISION };

// text information -----------------------------------
std::string text;
int ind;
char ch;
// token information ----------------------------------
type_token tokenType1;
operation tokenType2;
double tokenNumber;
// error information ----------------------------------
bool stateErr;  // false - not error, true - error.
//-----------------------------------------------------


/*!
 * @brief Subroutine to read the next token from the stream.\n
 * Note #1: Token information is stored in global variables.\n
 * Note #2: The subroutine does not affect the 'stateErr' flag, if the token is not defined, then the 'tokenType1' variable contains the value 'NONE'.
 */
void token()
{
    // checking end of line
    if( ind >= text.length() ) { tokenType1 = END; return; }

    // character reading
    nextSymbol

    // check for operation symbol
    tokenType1 = OPERATION;
    if( ch == '+' ) { tokenType2 = PLUS; return; }
    if( ch == '-' ) { tokenType2 = MINUS; return; }
    if( ch == '*' ) { tokenType2 = MULTIPLICATION; return; }
    if( ch == '/' ) { tokenType2 = DIVISION; return; }

    // checking for a number
    if( (ch=='.') || (ch>='0') && (ch<='9') )
    {
        std::string numTxt = "";
        bool decimalSeparator = false;
        int countOfDigits = 0;
        backSymbol
        while( ind < text.length() )
        {
            nextSymbol

            if ((ch == '.') && !decimalSeparator)
                decimalSeparator = true;
            else if ((ch >= '0') && (ch <= '9'))
                countOfDigits++;
            else {
                // other characters.
                backSymbol
                break;
            }

            numTxt += ch;
        }
        if( countOfDigits == 0 )
        {
            tokenType1 = NONE;
            return;
        }

        tokenNumber = std::stod(numTxt);
        tokenType1 = NUMBER;
        return;
    }

    // token not defined
    tokenType1 = NONE;
}

/*!
 * @brief A function to read a number from a stream.
 * @return Returns a number from the stream when 'stateErr' is true.
 */
double numberRead()
{
    bool minusSignNumbers;

    minusSignNumbers = false;  // Let's say that there is no unary operation, that is, by default, the plus sign.
    token();
    if( tokenType1 == OPERATION )
    {
        // handling unary operation.
        stateErr = (tokenType2 != PLUS) && (tokenType2 != MINUS);
        errorExit
        minusSignNumbers = (tokenType2 == MINUS);
        token();
    }
    stateErr = (tokenType1 != NUMBER);
    errorExit
    if( minusSignNumbers )
        tokenNumber = -tokenNumber;

    return tokenNumber;
}

/*!
 * @brief A function to calculate the value of an expression in the form “<number-1><action><number-2>” (without spaces).
 * @param text_ - given expression.
 * @return Returns the value of the expression when 'state Err' is true.
 */
double calculation(std::string &text_)
{
    double number1,number2;
    operation op;

    // initialization
    text = text_;
    ind = 0;
    //stateErr = false;

    // parsing
    //-----------------------------------------
    // reading the first number
    number1 = numberRead();
    errorExit
    //-----------------------------------------
    // read operation
    token();
    stateErr = (tokenType1 != OPERATION);
    errorExit
    op = tokenType2;
    //-----------------------------------------
    // second number reading
    number2 = numberRead();
    errorExit
    //-----------------------------------------
    // reading end of line
    token();
    stateErr = (tokenType1 != END);
    errorExit
    //-----------------------------------------

    // calculation
    if( op == PLUS )
        return number1 + number2;
    if( op == MINUS )
        return number1 - number2;
    if( op == MULTIPLICATION )
        return number1 * number2;
    if( op == DIVISION ) {
        if (number2 == 0) {
            stateErr = true;
            return 0;
        } else
            return number1 / number2;
    }

    return 0;
}



int main() {
    std::string input;
    double result;

    // data input
    std::cout << "Enter an arithmetic expression:";
    std::cin >> input;

    // work
    result = calculation(input);

    // result output
    std::cout << "Result: " << (stateErr?"Syntax error in expression.":std::to_string(result)) << std::endl;

    return 0;
}

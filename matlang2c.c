#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
char * varlist[500];//list of variables
char * vardimlist[500];//list of vectors and matrices
int  dimlistx[500];//list of x dimensions of matrices
int  dimlisty[500];//list of y dimensions of matrices (dimlistx,dimlisty and vardimlist have the same index for the same element and its dimensions)
int typelist[500];//list of types of elements(varlist and typelist have the same index for element and its corresponding type )
int isTerm(char * tokenline[], int s, int e);
int isMoreterms(char * tokenline[], int s, int e);
int isFactor(char * tokenline[], int s, int e);
int isMoreFactors(char * tokenline[], int s, int e);
int isExpression(char * tokenline[], int s, int e);
int * MatrixSizeCheck2(char * tokenline[],  int s, int e);
int * MatrixSizeCheck3(char * tokenline[],  int s, int e);
int * MatrixSizeCheck4(char * tokenline[],  int s, int e);
int * MatrixSizeCheck5(char * tokenline[],  int s, int e);
int linelenghts[500];//list to hold line lenghts
char * processedtokenline[500][500];//list to hold and modify the output
int processlinenum;//number of line that is being processed
int check[500][500];//list to check whether that location is processed or not
int varcount = 0;//number of variables

void ERROR(int linenum){//prints error message
    printf("Error (Line %d)",linenum+1);
    exit(0);
}
void initilize(FILE * out){//initilizes the output file
    fprintf(out,"#include <stdio.h>\n"
                "#include <string.h>\n"
                "#include <ctype.h>\n"
                "#include <stdlib.h>\n"
                "#include <math.h>\n"
                "\n"
                "int dimx[1500];\n"
                "int dimy[1500];\n"
                "double *  dimlist[500];\n"
                "int dimpos = 0;\n"
                "\n"
                "void printsep(){\n"
                "    printf(\"------------\\n\");\n"
                "}\n"
                "void print(double element){\n"
                "   if(fabs(((int)element) - element) < 0.0000009){\n"
                "       printf(\"%%d\\n\",(int)element);\n"
                "       return;\n"
                "   }\n"
                "   printf(\"%%f\\n\",element);\n"
                "}\n"
                "\n"
                "int getlenghtx(double * M){\n"
                "    for(int i = 0; i < 1500; i++){\n"
                "        if(dimlist[i] == M){\n"
                "            return dimx[i];\n"
                "        }\n"
                "    }\n"
                "}\n"
                "int getlenghty(double  * M){\n"
                "    for(int i = 0; i < 1500; i++){\n"
                "        if(dimlist[i] == M){\n"
                "            return dimy[i];\n"
                "        }\n"
                "    }\n"
                "}\n"
                "void pushlenght(double * M, int x, int y){\n"
                "    dimx[dimpos] = x;\n"
                "    dimy[dimpos] = y;\n"
                "    dimlist[dimpos] = M;\n"
                "    dimpos++;\n"
                "\n"
                "}\n"
                " double * matrixMultp(double * M1, double * M2){\n"
                "    int x1 = getlenghtx(M1);\n"
                "    int y1 = getlenghty(M1);\n"
                "    int x2 = getlenghtx(M2);\n"
                "    int y2 = getlenghty(M2);\n"
                "    int res = x1 * y2;\n"
                "    double * MM = malloc(sizeof(double) *  res);\n"
                "    double * result = MM;\n"
                "    for (int i = 0; i < x1; i++) {\n"
                "        for (int j = 0; j < y2; j++) {\n"
                "            double sum = 0;\n"
                "            for (int k = 0; k < y1; k++)\n"
                "                sum = sum + M1[i * y1 + k] * M2[k * y2 + j];\n"
                "            MM[i * y2 + j] = sum;\n"
                "        }\n"
                "    }\n"
                "    pushlenght(result,x1,y2);\n"
                "    return result;\n"
                "}\n"
                "double * matrixScalar(double s, double * M){\n"
                "    double * MM = malloc(sizeof(double) *  getlenghtx(M) * getlenghty(M));\n"
                "    double * result = MM;\n"
                "    int k = getlenghtx(M) * getlenghty(M);\n"
                "    double temp;\n"
                "    for(int i = 0; i < k; i++){\n"
                "        temp = s * M[i];\n"
                "        MM[i] = temp;\n"
                "    }\n"
                "    pushlenght(result, getlenghtx(M), getlenghty(M));\n"
                "    return result;\n"
                "}\n"
                "\n"
                "double * matrixSum(double * M1 , double  * M2){\n"
                "    double * MM = malloc(sizeof(double) *  getlenghtx(M1) * getlenghty(M1));\n"
                "    double * result = MM;\n"
                "    int k = getlenghtx(M1) * getlenghty(M1);\n"
                "    for(int i = 0; i < k; i++){\n"
                "        MM[i] = M1[i] + M2[i];\n"
                "    }\n"
                "    pushlenght(result, getlenghtx(M1), getlenghty(M1));\n"
                "    return result;\n"
                "\n"
                "}\n"
                "\n"
                "\n"
                "double * matrixDiff(double * M1 , double  * M2){\n"
                "    double * MM = malloc(sizeof(double) *  getlenghtx(M1) * getlenghty(M1));\n"
                "    double * result = MM;\n"
                "    int k = getlenghtx(M1) * getlenghty(M1);\n"
                "    for(int i = 0; i < k; i++){\n"
                "        MM[i] = M1[i] - M2[i];\n"
                "    }\n"
                "    pushlenght(result, getlenghtx(M1), getlenghty(M1));\n"
                "    return result;\n"
                "}\n"
                "\n"
                "double choose(double e1, double e2, double e3, double e4){\n"
                "    if(e1 == 0){\n"
                "        return e2;\n"
                "    }\n"
                "    if(e1 > 0){\n"
                "        return e3;\n"
                "    }\n"
                "    if(e1 < 0){\n"
                "        return e4;\n"
                "    }\n"
                "}\n"
                "\n"
                "double * trM(double * M){\n"
                "    double * MM = malloc(sizeof(double) *  getlenghtx(M) * getlenghty(M));\n"
                "    int x,y;\n"
                "    double * result = MM;\n"
                "    for(int i = 0; i < getlenghty(M) * getlenghtx(M); i++){\n"
                "        y = i / getlenghty(M);\n"
                "        x = i %% getlenghty(M);\n"
                "\n"
                "        MM[getlenghtx(M)*x+y] = M[i];\n"
                "\n"
                "    }\n"
                "    pushlenght(result, getlenghty(M), getlenghtx(M));\n"
                "    return result;\n"
                "\n"
                "}\n"
                "double tr(double x){\n"
                "    return x;\n"
                "}\n"
                "int main() {\n");

}




int id_finder(char * element){// checks whether the varible exists or not
    for(int i = 0; i < 500; i++){
        if(varlist[i] == NULL){
            break;
        }
        if(strcmp(element,varlist[i]) == 0){
            return i;
        }
    }
    return -1;
}
int min(int a, int b){// minimum function
    if(a == -1){
        return b;
    }
    else if(b == -1){
        return a;
    }
    else{
        if(a > b){
            return b;
        }
        else{
            return a;
        }
    }
}
int finder(char * element ,char * tokenline[],int s, int e);
int isnumber(char * element){//checks whether the element is a real number or not
    int isdouble = 0;
    for(int i = 0; i < strlen(element) ; i++){
        if(i == 0 && element[i] == '.' ){
            return 0;
        }
        if(element[i] == '.' ){
            if(isdouble == 0){
                isdouble = 1;
                continue;
            }
            else{
                return 0;
            }
        }
        if(isdigit(element[i]) == 0){
            return 0;
        }
    }
    return 1;
}
int finder(char * element ,char * tokenline[],int s, int e){// finds element in the given range and returns the index
    for(int i = s; i <= e ; i++){
        if(strcmp(element,tokenline[i]) == 0){
            return i;
        }
    }
    return -1;
}


int isLegit(char * varname){//checks whether the varname is legit or not
    if(isdigit(varname[0]) != 0){
        return -1;
    }
    for(int i = 0; i < 500; i++){
        if(varname[i] == '\0'){
            break;
        }
        if(isalpha(varname[i]) == 0){
            if(isdigit(varname[i]) == 0){
                if(varname[i] != '_'){
                    return -1;
                }
            }
        }

    }
    return 1;
}
int gettype(char * element ){//returns the type of element
    for(int i = 0; i < varcount; i++){
        if(strcmp(varlist[i],element) == 0){
            return typelist[i];
        }
    }
    return 0;

}
int  getdimensionsx(char * element){//returns the x dimension of given matrix
    for(int i = 0; i < 500; i++){
        if(strcmp(vardimlist[i],element) == 0){
            return dimlistx[i];
        }
    }
    return 0;
}
int  getdimensionsy(char * element){//returns the y dimension of given matrix
    for(int i = 0; i < 500; i++){
        if(strcmp(vardimlist[i],element) == 0){
            return dimlisty[i];
        }
    }
    return 0;
}
void pushtype(char * element , int  freepos, int type){// pushes the element to the typelist and varlist
    varlist[freepos] = strdup(element);
    typelist[freepos] = type;
    varcount++;

}
void pushdimensions(char * element, int  freepos,int a, int b){//pushes matrix's dimensions to the dimension lists
    vardimlist[freepos] = strdup(element);
    dimlistx[freepos] = a;
    dimlisty[freepos] = b;

}
int tr_count = 0;
int p_count = 0;
int * MatrixSizeCheck1(char * tokenline[], int s, int e);
int * MatrixSizeCheck6(char * tokenline[], int s, int e);
int * MatrixSizeCheck(char * tokenline[], int s, int e){//Recursive function to check matrix sizes
    static int  size[2];
    int error = -1;
    if(finder("+", tokenline, s, e) == -1){//tries to find + operator
        if(MatrixSizeCheck1(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else if (MatrixSizeCheck1(tokenline, s , e)[0] != -1){//if there is no + operator it calls the function that finds - operator
            return MatrixSizeCheck1(tokenline, s , e);
        }
        else{
            size[0] = -1;
            return size;
        }
    }
    int opindex = finder("+", tokenline, s, e);
    while(MatrixSizeCheck1(tokenline, s, opindex-1)[0] == -1){//if the left side of the operator is not a - operator function(it happens when there are parenthesis), it looks for another operator
        opindex = finder("+", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck1(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
        return MatrixSizeCheck1(tokenline, s, e);


    }
    else{
        if(MatrixSizeCheck3(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    if(MatrixSizeCheck1(tokenline,s,opindex-1)[0] != MatrixSizeCheck3(tokenline,opindex+1,e)[0] || MatrixSizeCheck1(tokenline,s,opindex-1)[1] != MatrixSizeCheck3(tokenline,opindex+1,e)[1]){
        //compares the dimensions of the matrices
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){//processes the input, omits the + and replaces them with corresponding function
            if(strcmp(tokenline[opindex],"+")  == 0){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixSum(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");
                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
            }
            check[processlinenum][opindex] = 1;
        }
        return MatrixSizeCheck1(tokenline,s,opindex-1);
    }
}
int * MatrixSizeCheck1(char * tokenline[], int s, int e){//Recursive function to check matrix sizes
    static int  size[2];
    int error = -1;
    if(finder("-", tokenline, s, e) == -1 ){//tries to find  - operator
        if(MatrixSizeCheck2(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else if (MatrixSizeCheck2(tokenline, s , e)[0] != -1){//if there is no - operator it calls the function corresponding to Term
            return MatrixSizeCheck2(tokenline, s , e);
        }
        else{
            size[0] = -1;
            return size;
        }
    }
    int opindex = finder("-", tokenline, s, e);;
    while(MatrixSizeCheck2(tokenline, s, opindex-1)[0] == -1){//if the left side of the operator is not a term(it happens when there are parenthesis), it looks for another operator
        opindex = finder("-", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck2(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
        return MatrixSizeCheck2(tokenline, s, e);


    }
    else{
        if(MatrixSizeCheck6(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    if(MatrixSizeCheck2(tokenline,s,opindex-1)[0] != MatrixSizeCheck6(tokenline,opindex+1,e)[0] || MatrixSizeCheck2(tokenline,s,opindex-1)[1] != MatrixSizeCheck6(tokenline,opindex+1,e)[1]){
        //compares the dimensions of the matrices
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){//processes the input, omits the  - and replaces them with corresponding functions
            if(strcmp(tokenline[opindex],"-")  == 0){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixDiff(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");

                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
            }
            check[processlinenum][opindex] = 1;
        }
        return MatrixSizeCheck2(tokenline,s,opindex-1);
    }
}
int * MatrixSizeCheck2(char * tokenline[],  int s, int e){//Matrix size check corresponding to Term Check
    static int size[2];
    int error = -1;
    if(finder("*", tokenline, s, e) == -1 ){//Looks for *
        if(MatrixSizeCheck4(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else{
            return MatrixSizeCheck4(tokenline, s , e);//if there is no * then it calls the function corresponding to Factor
        }
    }
    int opindex = finder("*", tokenline, s, e);
    while(MatrixSizeCheck4(tokenline, s, opindex-1)[0] == -1){//if the left side of the operator is not a Factor, it looks for another one
        opindex = finder("*", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck4(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[0] == -2 ){
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck5(tokenline, opindex+1, e);//if both factors are scalar
        }
        else{
            if(check[processlinenum][opindex] != 1){//if only one of them is scalar then it omits the * and puts the corresponing function
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixScalar(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");

                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);

                check[processlinenum][opindex] = 1;
            }
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){//Same thing just reversed
            if(check[processlinenum][opindex] != 1){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixScalar(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i =  opindex+1; i <=  e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");

                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);

                check[processlinenum][opindex] = 1;
            }
            return MatrixSizeCheck4(tokenline,s,opindex-1);
        }
    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[0] == -2 ){//double check
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
        else{
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck4(tokenline,s,opindex-1);
        }
    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[1] != MatrixSizeCheck5(tokenline,opindex+1,e)[0]){//compares the dimensions of the multiplied matrices
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){//replaces the * with the corresponding function
            char * a;
            a = malloc(sizeof(char) * 1000);
            sprintf(a, "matrixMultp(");
            processedtokenline[processlinenum][opindex] = " ";
            for(int i =  s; i <=  opindex - 1; i++ ){
                strcat(a, strdup(processedtokenline[processlinenum][i]));
                processedtokenline[processlinenum][i] = " ";
            }
            strcat(a,",");

            for(int i = opindex + 1; i <= e; i++ ){
                strcat(a, strdup(processedtokenline[processlinenum][i]));
                processedtokenline[processlinenum][i] = " ";
            }
            strcat(a,")");
            processedtokenline[processlinenum][s] = strdup(a);
            check[processlinenum][opindex] = 1;
        }
        size[0] = MatrixSizeCheck4(tokenline,s,opindex-1)[0];//returns the x dimension of the matrix that is on the left and the y dimension of the matrix that is on the right
        size[1] = MatrixSizeCheck5(tokenline,opindex+1,e)[1];
    return size;
    }
}
int * MatrixSizeCheck3(char * tokenline[],  int s, int e){//MoreTerms, does exactly the same thing as the MatrixSizeCheck
    static int  size[2];
    int error = -1;
    if(finder("+", tokenline, s, e) == -1 ){
        if(MatrixSizeCheck1(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else if (MatrixSizeCheck1(tokenline, s , e)[0] != -1){
            return MatrixSizeCheck1(tokenline, s , e);
        }
        else{
            size[0] = -1;
            return size;
        }
    }
    int opindex = finder("+", tokenline, s, e);
    while(MatrixSizeCheck1(tokenline, s, opindex-1)[0] == -1){
        opindex = finder("+", tokenline, opindex+1, e);
        if(opindex == -1) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck1(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
        return MatrixSizeCheck1(tokenline, s, e);
    }
    else{
        if(MatrixSizeCheck3(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    if(MatrixSizeCheck1(tokenline,s,opindex-1)[0] != MatrixSizeCheck3(tokenline,opindex+1,e)[0] || MatrixSizeCheck1(tokenline,s,opindex-1)[1] != MatrixSizeCheck3(tokenline,opindex+1,e)[1]){
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){
            if(strcmp(tokenline[opindex],"+")  == 0){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixSum(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");

                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
            }
            check[processlinenum][opindex] = 1;
        }
        return MatrixSizeCheck1(tokenline,s,opindex-1);
    }
}
int * MatrixSizeCheck4(char * tokenline[],  int s, int e){//Function corresponding to isFactor
    //If the thing between s and e are scalar it returns an array of 2 integers with first element -2, else it returns an array of 2 integers with first element as x dimension and the second as y
    static int size[2];
    if(strcmp(tokenline[s],"(") == 0){
        if(strcmp(tokenline[e],")") != 0){
            size[0] = -1;
            return size;
        }
        else{
            return MatrixSizeCheck(tokenline, s+1, e-1);
        }
    }
    if(strcmp(tokenline[s],"tr") == 0){//transpose function
        if(strcmp(tokenline[s+1],"(") != 0){
            size[0] = -1;
            return size;
        }
        else if(strcmp(tokenline[e],")") != 0){
            size[0] = -1;
            return size;
        }
        else{
            if(MatrixSizeCheck(tokenline, s+2, e-1)[0] == -1){
                size[0] = -1;
                return size;
            }
            else if(MatrixSizeCheck(tokenline, s+2, e-1)[0] == -2){
                size[0] = -2;
                return size;
            }
            else{
                if(check[processlinenum][s] != 1){
                    char * a;
                    a = malloc(sizeof(char) * 1000);
                    sprintf(a, "trM");//there are 2 transpose functions in the out file, one for transposing matrices and the other is for transposing scalars, trM is for matrices the unedited one is for scalars
                    processedtokenline[processlinenum][s] = strdup(a);
                    check[processlinenum][s] = 1;
                }
                static int size1[2];
                size1[0] = MatrixSizeCheck(tokenline, s+2, e-1)[1];
                size1[1] = MatrixSizeCheck(tokenline, s+2, e-1)[0];
                return size1;
            }
        }
    }
    if(s==e){//scalar or vector or matrix or a number
        if(isnumber(tokenline[s]) == 1){
            size[0] = -2;
            return size;
        }
        else if(gettype(tokenline[s]) == 1){
            size[0] = -2;
            return size;
        }
        else{
            size[0] = getdimensionsx(tokenline[s]);
            size[1] = getdimensionsy(tokenline[s]);
            return size;
        }
    }
    if(strcmp(tokenline[s],"choose") == 0){//choose always returns scalar
        size[0] = -2;
        return size;
    }
    if(strcmp(tokenline[s],"sqrt") == 0){//sqrt always returns scalar
        size[0] = -2;
        return size;
    }
    if(gettype(tokenline[s]) == 3){//element of a matrix is a scalar
        if(strcmp(tokenline[s+1],"[") == 0){
            size[0] = -2;
            return size;
        }
    }
    if(gettype(tokenline[s]) == 2){//element of a vector is a scalar
        if(strcmp(tokenline[s+1],"[") == 0){
            size[0] = -2;
            return size;
        }
    }
    return size;
}
int * MatrixSizeCheck5(char * tokenline[],  int s, int e){//Exactly the same as MatrixSizeCheck2
    static int size[2];
    int error = -1;
    if(finder("*", tokenline, s, e) == -1 ){
        if(MatrixSizeCheck4(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else if(MatrixSizeCheck4(tokenline, s , e)[0] == -2){
            return MatrixSizeCheck4(tokenline, s , e);
        }
        else{
            return MatrixSizeCheck4(tokenline, s , e);
        }
    }
    int opindex = finder("*", tokenline, s, e);
    while(MatrixSizeCheck4(tokenline, s, opindex-1)[0] == -1){
        opindex = finder("*", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck4(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else{
            return MatrixSizeCheck4(tokenline, s, e);
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }

    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[0] == -2 ){
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
        else{
            if(check[processlinenum][opindex] != 1){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixScalar(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");
                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
                check[processlinenum][opindex] = 1;
            }
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            if(check[processlinenum][opindex] != 1){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixScalar(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i =  opindex+1; i <=  e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
                check[processlinenum][opindex] = 1;
            }
            return MatrixSizeCheck4(tokenline,s,opindex-1);
        }
    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[0] == -2 ){
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
        else{
            return MatrixSizeCheck5(tokenline, opindex+1, e);
        }
    }
    else{
        if(MatrixSizeCheck5(tokenline, opindex+1, e)[0] == -2 ){
            return MatrixSizeCheck4(tokenline,s,opindex-1);
        }
    }
    if(MatrixSizeCheck4(tokenline,s,opindex-1)[1] != MatrixSizeCheck5(tokenline,opindex+1,e)[0]){
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){
            char * a;
            a = malloc(sizeof(char) * 1000);
            sprintf(a, "matrixMultp(");
            processedtokenline[processlinenum][opindex] = " ";
            for(int i =  s; i <=  opindex - 1; i++ ){
                strcat(a, strdup(processedtokenline[processlinenum][i]));
                processedtokenline[processlinenum][i] = " ";
            }
            strcat(a,",");
            for(int i = opindex + 1; i <= e; i++ ){
                strcat(a, strdup(processedtokenline[processlinenum][i]));
                processedtokenline[processlinenum][i] = " ";
            }
            strcat(a,")");
            processedtokenline[processlinenum][s] = strdup(a);
            check[processlinenum][opindex] = 1;
        }


        size[0] = MatrixSizeCheck4(tokenline,s,opindex-1)[0];
        size[1] = MatrixSizeCheck5(tokenline,opindex+1,e)[1];
        return size;
    }
}
int * MatrixSizeCheck6(char * tokenline[], int s, int e){//Does exactly the same thing as MatrixSizeCheck1
    static int  size[2];
    int error = -1;
    if(finder("-", tokenline, s, e) == -1 ){//tries to find - operator
        if(MatrixSizeCheck2(tokenline, s , e)[0] == -1){
            size[0] = -1;
            return size;
        }
        else if (MatrixSizeCheck2(tokenline, s , e)[0] != -1){//if there is no  - operator it calls the function corresponding to Term
            return MatrixSizeCheck2(tokenline, s , e);
        }
        else{
            size[0] = -1;
            return size;
        }
    }
    int opindex = finder("-", tokenline, s, e);;
    while(MatrixSizeCheck2(tokenline, s, opindex-1)[0] == -1){//if the left side of the operator is not a term(it happens when there are parenthesis), it looks for another operator
        opindex = finder("-", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(MatrixSizeCheck2(tokenline, s, e)[0] == -1){
            size[0] = -1;
            return size;
        }
        return MatrixSizeCheck2(tokenline, s, e);


    }
    else{
        if(MatrixSizeCheck6(tokenline, opindex+1, e)[0] == -1){
            size[0] = -1;
            return size;
        }
    }
    if(MatrixSizeCheck2(tokenline,s,opindex-1)[0] != MatrixSizeCheck6(tokenline,opindex+1,e)[0] || MatrixSizeCheck2(tokenline,s,opindex-1)[1] != MatrixSizeCheck6(tokenline,opindex+1,e)[1]){
        //compares the dimensions of the matrices
        size[0] = -1;
        return size;
    }
    else{
        if(check[processlinenum][opindex] != 1){//processes the input, omits the - and replaces them with corresponding function
            if(strcmp(tokenline[opindex],"-")  == 0){
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "matrixDiff(");
                processedtokenline[processlinenum][opindex] = " ";
                for(int i = s; i <= opindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,",");

                for(int i = opindex+1; i <= e; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,")");
                processedtokenline[processlinenum][s] = strdup(a);
            }
            check[processlinenum][opindex] = 1;
        }
        return MatrixSizeCheck2(tokenline,s,opindex-1);
    }
}

void spacer(char * line ){//puts spaces between elements
    if(strcmp(&line[0],"\n") ==0){
        return;
    }
    char  templine[256];

    memset(templine,0,256);
    for(int i = 0; i < 256; i++){
        if(line[i] == '\0'){
            break;
        }


        if(isalnum(line[i])){
            if(line[i+1] == '_'){
                continue;
            }
        }
        if(line[i] == '_'){
            if(isalnum(line[i+1])){
                continue;
            }
        }
        if( line[i] ==')' || line[i] ==']' || line[i] =='}' || line[i] =='{'|| line[i] =='('|| line[i] =='['|| line[i] =='+'|| line[i] =='-'|| line[i] =='*'){
            strncpy(templine, line, i+1);
            templine[i+1] = '\0';
            strcat(templine, " ");
            strcat(templine, line +i+1 );
            strncpy(line, templine, 256);
            continue;
        }
        if(isspace(line[i])){
            continue;
        }
        if(line[i] ==  '.'){
            continue;
        }
        if(line[i+1] ==  '.'){
            continue;
        }
        if(isalnum(line[i]) || line[i] == '_'){
            if(isalnum(line[i+1]) ){

            } else{
                strncpy(templine, line, i+1);
                templine[i+1] = '\0';
                strcat(templine, " ");
                strcat(templine, line +i+1 );
                strncpy(line, templine, 256);

            }
        }
        else{
            if(isalnum(line[i+1]) || line[i] == '_'){
                strncpy(templine, line, i+1);
                templine[i+1] = '\0';
                strcat(templine, " ");
                strcat(templine, line + i+1);
                strncpy(line, templine, 256);
            }
        }
    }
}
void tokenizer(char * element, char * tokenline[], int tokenpos){//Used to tokenize the lines now just adds them to tokenline list
    tokenline[tokenpos] = strdup(element);
}
int isExpression(char * tokenline[], int s, int e){//Checks whether the interval between s and e is expression or not, also checks type compatibility
    int error = -1;
    if(finder("+", tokenline, s, e) == -1 && finder("-", tokenline, s, e) == -1 ){//looks for + or -
        if(isTerm(tokenline, s , e) == -1){
            return -1;
        }
        else if (isTerm(tokenline, s , e) == 1){//it is a full term if no + or -
            return 1;
        }
        else{
            return 2;
        }
    }
    int opindex1 = finder("+", tokenline, s, e);
    int opindex2 = finder("-", tokenline, s, e);
    int opindex = min(opindex1, opindex2);
    while(isTerm(tokenline, s, opindex-1) == -1){//if the left side of the operator is not a term it looks for another operator
        opindex1 = finder("+", tokenline, opindex+1, e);
        opindex2 = finder("-", tokenline, opindex+1, e);
        opindex = min(opindex1, opindex2);
        if(opindex1 == -1 && opindex2 == -1) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(isTerm(tokenline, s, e) == -1){
            return -1;
        }
        return isTerm(tokenline, s, e);
    }
    else{
        if(isMoreterms(tokenline, opindex+1, e) == -1){//Checks the right side of the operator
            return -1;
        }
    }
    if(isTerm(tokenline,s,opindex-1) != isMoreterms(tokenline,opindex+1,e)){//checks the type compatibility
        return -1;
    }
    else{
        return isTerm(tokenline,s,opindex-1);
    }
}
int isTerm(char * tokenline[], int s, int e){//Checks whether the interval between s and e is a term or not
    int error = -1;
    if(finder("*", tokenline, s, e) == -1 ){//looks for *
        if(isFactor(tokenline, s , e) == -1){
            return -1;
        }
        else if (isFactor(tokenline, s , e) == 1){//if there is no * then it has to be a full Factor
            return 1;
        }
        else{
            return 2;
        }
    }
    int opindex = finder("*", tokenline, s, e);
    while(isFactor(tokenline, s, opindex-1) == -1){//checks for another * if the left side of the operator is not a Factor
        opindex = finder("*", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(isFactor(tokenline, s, e) == -1){
            return -1;
        }
    }
    else{
        if(isMoreFactors(tokenline, opindex+1, e) == -1){// Checks the right side of the operator
            return -1;
        }
    }
    if(isMoreFactors(tokenline,opindex+1,e) == 2 || isFactor(tokenline,s,opindex-1) == 2 ){//if one side of the operator is a matrix then the combination is also a matrix
        return 2;
    }
    return isFactor(tokenline,s,opindex-1);//if both sides are scalar it returns scalar
}
int isMoreterms(char * tokenline[], int s, int e){//Exactly the same as the isExpression function
    int error = -1;
    if(finder("+", tokenline, s, e) == -1 && finder("-", tokenline, s, e) == -1 ){
        if(isTerm(tokenline, s , e) == -1){
            return -1;
        }
        else if (isTerm(tokenline, s , e) == 1){
            return 1;
        }
        else{
            return 2;
        }
    }
    int opindex1 = finder("+", tokenline, s, e);
    int opindex2 = finder("-", tokenline, s, e);
    int opindex = min(opindex1, opindex2);
    while(isTerm(tokenline, s, opindex-1) == -1){
        opindex1 = finder("+", tokenline, opindex+1, e);
        opindex2 = finder("-", tokenline, opindex+1, e);
        opindex = min(opindex1, opindex2);
        if(opindex1 == -1 && opindex2 == -1) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(isTerm(tokenline, s, e) == -1){
            return -1;
        }
        return isTerm(tokenline, s, e);
    }
    else{
        if(isMoreterms(tokenline, opindex+1, e) == -1){
            return -1;
        }
    }
    if(isTerm(tokenline,s,opindex-1) != isMoreterms(tokenline,opindex+1,e)){
        return -1;
    }
    else{
        return isTerm(tokenline,s,opindex-1);
    }
}
int isFactor(char * tokenline[], int s, int e){//checks whether the interval between s and e is a Factor or not
    if(strcmp(tokenline[s],"(") == 0){//Could be a expression in parenthesis
        if(strcmp(tokenline[e],")") == 0){
            if(isExpression(tokenline,s+1,e-1) == -1){
                return -1;
            }
            else{
                return isExpression(tokenline,s+1,e-1);
            }
        }
        else{
            return -1;
        }
    }
    if(s == e){//could be an identifer or a number
        if(id_finder(tokenline[s]) != -1){
            if(gettype(tokenline[s]) == 1){
                return 1;
            }
            else{
                return 2;
            }
        }
        else{
            if(isnumber(tokenline[s]) != 0){
                return 1;
            }
            else{
                return -1;
            }
        }
    }
    if(strcmp("[",tokenline[s+1]) == 0){//could be an element of a matrix or a vector
        if(strcmp("]",tokenline[e]) != 0){
            return -1;
        }
        if(gettype(tokenline[s]) == 3){//vector
            if(isExpression(tokenline,s+2,e-1) == 1){
                if(s+2 == e-1){
                    if(isnumber(tokenline[s+2]) == 1){
                        if(getdimensionsx(tokenline[s]) < atoi(tokenline[s+2])){
                            return -1;
                        }
                    }
                }
                if(check[processlinenum][s] != 1){//changes the insides of the braces to make it suitable to print to the output
                    char *  tempstr;
                    tempstr = malloc(sizeof(char) * 1000);
                    sprintf(tempstr,"(int)floor(");
                    for(int i = s+2; i <= e-1; i++){
                        strcat(tempstr,processedtokenline[processlinenum][i]);
                        processedtokenline[processlinenum][i] = " ";
                    }
                    strcat(tempstr,"-1");
                    strcat(tempstr,")");
                    processedtokenline[processlinenum][e-1] = tempstr;
                    check[processlinenum][s] = 1;
                }
                return 1;
            }
            else{
                return -1;
            }
        }
        if(gettype(tokenline[s]) == 2){//matrix
            int commaindex = finder(",",tokenline,s+2,e-1);
            int error = -1;
            while(isExpression(tokenline, s+2, commaindex-1) != 1){//finds the comma
                commaindex = finder(",",tokenline,commaindex+1,e-1);
                if(commaindex == -1) {
                    error = 1;
                    break;
                }
            }
            if(error == 1){
                return -1;
            }
            if(isExpression(tokenline,commaindex+1,e-1) != 1){
                return -1;
            }
            if(check[processlinenum][commaindex] != 1){//changes the insides of the braces to make it suitable to print to the output
                char * a;
                a = malloc(sizeof(char) * 1000);
                sprintf(a, "(int)floor(%d*(", getdimensionsy(tokenline[s]));
                for(int i = s+2; i <= commaindex - 1; i++ ){
                    strcat(a, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(a,"-1");
                strcat(a,")+");
                processedtokenline[processlinenum][commaindex] = " ";
                char * b;
                b = malloc(sizeof(char) * 1000);
                sprintf(b,"");


                for(int i = commaindex+1; i <= e-1; i++ ){
                    strcat(b, strdup(processedtokenline[processlinenum][i]));
                    processedtokenline[processlinenum][i] = " ";
                }
                strcat(b,"-1");
                strcat(a,b);
                strcat(a,")");
                processedtokenline[processlinenum][s+2] = strdup(a);
                check[processlinenum][commaindex] = 1;
            }
            if(commaindex + 1 == e - 1){
                if(isnumber(tokenline[commaindex + 1]) == 1){
                    if(atoi(tokenline[commaindex+1]) > getdimensionsy(tokenline[s])){
                        return -1;
                    }
                }
            }
            if(commaindex - 1 == s + 2){
                if(isnumber(tokenline[commaindex - 1]) == 1){
                    if(atoi(tokenline[commaindex-1]) > getdimensionsx(tokenline[s])){
                        return -1;
                    }
                }
            }
            return 1;
        }
    }
    if(strcmp(tokenline[s],"sqrt") == 0){//could be a sqrt function
        if(strcmp(tokenline[s+1],"(") != 0){
            return -1;
        }
        else if(strcmp(tokenline[e],")") != 0){
            return -1;
        }
        else{
            if(isExpression(tokenline,s+2,e-1) != 1){
                return -1;
            }
            else{
                return 1;
            }
        }
    }
    if(strcmp(tokenline[s],"choose") == 0){//could be a choose function
        if(strcmp(tokenline[s+1],"(") != 0){
            return -1;
        }
        else if(strcmp(tokenline[e],")") != 0){
            return -1;
        }
        else{//finds all 3 commas with scalar expressions between them
            int commaindex1 = finder(",",tokenline,s+2,e-1);
            int error = -1;
            while(isExpression(tokenline, s+2, commaindex1-1) != 1){
                commaindex1 = finder(",",tokenline,commaindex1+1,e-1);
                if(commaindex1 == -1) {
                    error = 1;
                    break;
                }
            }
            if(error == 1){
                return -1;
            }
            int commaindex2 = finder(",",tokenline,commaindex1+1,e-1);
            while(isExpression(tokenline, commaindex1+1, commaindex2-1) != 1){
                commaindex2 = finder(",",tokenline,commaindex2+1,e-1);
                if(commaindex2 == -1) {
                    error = 1;
                    break;
                }
            }
            if(error == 1){
                return -1;
            }
            int commaindex3 = finder(",",tokenline,commaindex2+1,e-1);
            while(isExpression(tokenline, commaindex2+1, commaindex3-1) != 1){
                commaindex3 = finder(",",tokenline,commaindex3+1,e-1);
                if(commaindex3 == -1) {
                    error = 1;
                    break;
                }
            }
            if(error == 1){
                return -1;
            }
            if(isExpression(tokenline,commaindex3+1,e-1) != 1){
                return -1;
            }
            return 1;
        }
    }
    if(strcmp(tokenline[s],"tr")==0){//could be a transpose function
        if(strcmp(tokenline[s+1],"(") != 0){
            return -1;
        }
        else if(strcmp(tokenline[e],")") != 0){
            return -1;
        }
        else{
            if(isExpression(tokenline,s+2,e-1) == 2){
                return 2;
            }
            else if(isExpression(tokenline,s+2,e-1) == 1){
                return 1;
            }
            else{
                return -1;
            }
        }
    }
    if(strcmp(tokenline[s],"{") == 0){//could be matrix/vector definition
        if(strcmp(tokenline[e],"}" ) != 0){
            return -1;
        }
        int num = getdimensionsx(tokenline[0]) * getdimensionsy(tokenline[0]);
        if(num != e-s-1){
            return -1;
        }
        else{
            for(int i = s+1; i < e; i++){
                if(isdigit(*tokenline[i]) == 0){
                    return -1;
                }
            }
            return 2;
        }
    }
    return -1;
}
int isMoreFactors(char * tokenline[], int s, int e){//Exactly the same as isTerm
    int error = -1;
    if(finder("*", tokenline, s, e) == -1 ){
        if(isFactor(tokenline, s , e) == -1){
            return -1;
        }
        else if (isFactor(tokenline, s , e) == 1){
            return 1;
        }
        else{
            return 2;
        }
    }
    int opindex = finder("*", tokenline, s, e);
    while(isFactor(tokenline, s, opindex-1) == -1){
        opindex = finder("*", tokenline, opindex+1, e);
        if(opindex == -1 ) {
            error = 1;
            break;
        }
    }
    if(error == 1){
        if(isFactor(tokenline, s, e) == -1){
            return -1;
        } else{
            return isFactor(tokenline, s, e);
        }
    }
    else{
        if(isMoreFactors(tokenline, opindex+1, e) == -1){
            return -1;
        }
    }
    if(isMoreFactors(tokenline,opindex+1,e) == 2 || isFactor(tokenline,s,opindex-1) == 2 ){
        return 2;
    }
    return isFactor(tokenline,s,opindex-1);
}
int main(int argc, char **argv) {
    FILE *fp;
    char line[256];
    fp = fopen(argv[1], "r");
    int freevarpos = 0;
    int freedimpos = 0;
    int linenum = 0;
    char * tokenline[500][500];
    while( fgets(line,256,fp) != NULL ) {//constructs the tokenline list
        int tokenpos = 0;
        int k = 0;
        char ** q = tokenline[linenum];
        spacer(line);
        char * token = strtok(line," ");
        while(token != NULL ){
            tokenizer(token,q,tokenpos);
            tokenpos++;
            if(strcmp(token,"\n") == 0){
                 k = 1;
            }
            token = strtok(NULL," ");
        }

        linenum++;
    }

    FILE *re;
    re = fopen(argv[1], "r");
    linenum = 0;
    while( fgets(line,256,re) != NULL ) {//constructs the processedtokenline list(it gave an error when I tried to read the fp again so I fixed it)
        //The difference between tokenline and processedtokenline is processedtokenline gets edited to be suitable for output while tokenline is to keep the original file
        int tokenpos = 0;
        int k = 0;
        char ** q = processedtokenline[linenum];
        spacer(line);
        char * token = strtok(line," ");
        while(token != NULL ){
            tokenizer(token,q,tokenpos);
            tokenpos++;
            if(strcmp(token,"\n") == 0){
                k = 1;
            }
            token = strtok(NULL," ");
        }

        linenum++;
    }
    for(int i = 0; i < linenum; i++){
        for(int a = 0 ; a < 500; a++) {
            if (tokenline[i][a] == NULL) {
                continue;
            }
        }

    }

    int counter;
    for(int i = 0; i <=linenum;i++){//computes the lenghts of each line
        counter = 0;
        if(tokenline[i][counter] == NULL){
            break;
        }
        while(strcmp(tokenline[i][counter],"\n") != 0) {

            counter++;
            if(tokenline[i][counter] == NULL){
                break;
            }
        }
        linelenghts[i] = counter;
    }
    tokenline[linenum-1][linelenghts[linenum-1]] = "\n";
    processedtokenline[linenum-1][linelenghts[linenum-1]] = "\n";
    linenum = 0;
    fclose(fp);
    FILE *fpc;
    fpc = fopen(argv[1], "r");
    int loop = 0;
    int exec = 0;
    while( fgets(line,256,fpc) != NULL ) {//reading the file again is unnecessary here but I was still getting familiar with C while I was writing here and by the time I realized it was too intertwined with the rest of the code
        processlinenum = linenum;
        if (line[0] == '\n') {//empty line
            linenum++;
            continue;
        }
        spacer(line);
        char *linelist = strtok(line, " ");
        if (*linelist == '#') {// comment
            linenum++;
            continue;
        }
        if (strcmp(&linelist[0], "scalar") == 0) {// scalar declaration
            if (exec == 1) {
                ERROR(linenum);
            }
            if (linelenghts[linenum] != 2) {
                ERROR(linenum);
            }
            linelist = strtok(NULL, " ");
            if (isLegit(linelist) < 0) {
                ERROR(linenum);
            }
            if (gettype(linelist) != 0) {
                ERROR(linenum);
            } else {
                pushtype(linelist, freevarpos, 1);
                freevarpos++;
                linenum++;
                continue;
            }
        }
        if (strcmp(&linelist[0], "vector") == 0) {// vector declaration
            if (exec == 1) {
                ERROR(linenum);
            }
            if (linelenghts[linenum] != 5) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][2], "[") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][4], "]") != 0) {
                ERROR(linenum);
            }
            if (isnumber(tokenline[linenum][3]) == 0) {
                ERROR(linenum);
            }
            linelist = strtok(NULL, " ");
            if (isLegit(linelist) < 0) {
                ERROR(linenum);
            }
            if (gettype(linelist) != 0) {
                ERROR(linenum);
            } else {
                pushtype(linelist, freevarpos, 3);
                freevarpos++;
                strtok(NULL, " ");
                int a = atoi(strtok(NULL, " "));
                if (a == 0) {
                    ERROR(linenum);
                }
                pushdimensions(linelist, freedimpos, a, 1);
                freedimpos++;
                linenum++;
                continue;
            }
        }
        if (strcmp(&linelist[0], "matrix") == 0) {//matrix declaration
            if (exec == 1) {
                ERROR(linenum);
            }
            if (linelenghts[linenum] != 7) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][4], ",") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][2], "[") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][6], "]") != 0) {
                ERROR(linenum);
            }
            if (isnumber(tokenline[linenum][3]) == 0) {
                ERROR(linenum);
            }
            if (isnumber(tokenline[linenum][5]) == 0) {
                ERROR(linenum);
            }
            linelist = strtok(NULL, " ");
            if (isLegit(linelist) < 0) {
                ERROR(linenum);
            }
            if (gettype(linelist) != 0) {
                ERROR(linenum);
            } else {
                pushtype(linelist, freevarpos, 2);
                freevarpos++;
                strtok(NULL, " ");
                int a = atoi(strtok(NULL, " "));
                if (a == 0) {
                    ERROR(linenum);
                }
                strtok(NULL, " ");
                int b = atoi(strtok(NULL, " "));
                if (b == 0) {
                    ERROR(linenum);
                }
                pushdimensions(linelist, freedimpos, a, b);
                freedimpos++;
                linenum++;
                continue;
            }

        }
        if (exec != 1) {//checks whether the definition part is over or not
            exec = 1;
        }
        if (strcmp(&linelist[0], "}") == 0) {//end of loop
            if (loop == 1) {
                loop = 0;
                linenum++;
                continue;
            } else {
                ERROR(linenum);
            }
            linenum++;
            continue;
        }
        if (strcmp(&linelist[0], "for") == 0) {//For statement
            //From here it just checks whether every token is in the right location or not
            if (loop == 1) {//nested loop is forbidden
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][1], "(") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][linelenghts[linenum] - 2], ")") != 0) {
                ERROR(linenum);
            }
            if (gettype(tokenline[linenum][2]) == 0) {
                ERROR(linenum);
            }
            if (gettype(tokenline[linenum][2]) == 2) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][linelenghts[linenum] - 1], "{") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][3], ",") == 0) {//double loop
                if (gettype(tokenline[linenum][4]) == 0) {
                    ERROR(linenum);
                } else if (gettype(tokenline[linenum][4]) == 2) {
                    ERROR(linenum);
                } else {
                    if (strcmp(tokenline[linenum][5], "in") != 0) {
                        ERROR(linenum);
                    } else {// finds the first colon
                        int semi11 = finder(":", tokenline[linenum], 6, linelenghts[linenum]);
                        if (semi11 == -1) {
                            ERROR(linenum);
                        }
                        if (isExpression(tokenline[linenum], 6, semi11 - 1) == 2) {
                            ERROR(linenum);
                        } else if (isExpression(tokenline[linenum], 6, semi11 - 1) != 1) {
                            ERROR(linenum);
                        } else {// finds the second colon
                            int semi12 = finder(":", tokenline[linenum], semi11 + 1, linelenghts[linenum]);
                            if (semi12 == -1) {
                                ERROR(linenum);
                            }
                            if (isExpression(tokenline[linenum], semi11 + 1, semi12 - 1) == 2) {
                                ERROR(linenum);
                            } else if (isExpression(tokenline[linenum], semi11 + 1, semi12 - 1) != 1) {
                                ERROR(linenum);
                            } else {// finds the comma between 2 loops' expressions
                                int commaindex = finder(",", tokenline[linenum], semi12 + 1,
                                                        linelenghts[linenum]);//finds the comma of the double loop
                                int error = 0;
                                while (isExpression(tokenline[linenum], semi12 + 1, commaindex - 1) != 1) {
                                    commaindex = finder(",", tokenline[linenum], commaindex + 1, linelenghts[linenum]);
                                    if (commaindex - 1) {
                                        error = 1;
                                        break;
                                    }
                                }
                                if (error == 1) {
                                    ERROR(linenum);
                                }
                                int semi21 = finder(":", tokenline[linenum], commaindex + 1,
                                                    linelenghts[linenum]);// finds the third colon
                                if (semi21 == -1) {
                                    ERROR(linenum);
                                }
                                if (isExpression(tokenline[linenum], commaindex + 1, semi21 - 1) == 2) {
                                    ERROR(linenum);
                                } else if (isExpression(tokenline[linenum], commaindex + 1, semi21 - 1) != 1) {
                                    ERROR(linenum);
                                } else {
                                    int semi22 = finder(":", tokenline[linenum], semi21 + 1,
                                                        linelenghts[linenum]);// finds the fourth colon
                                    if (semi22 == -1) {
                                        ERROR(linenum);
                                    }
                                    if (isExpression(tokenline[linenum], semi21 + 1, semi22 - 1) == 2) {
                                        ERROR(linenum);
                                    } else if (isExpression(tokenline[linenum], semi21 + 1, semi22 - 1) != 1) {
                                        ERROR(linenum);
                                    } else {
                                        if (isExpression(tokenline[linenum], semi22 + 1, linelenghts[linenum] - 3) ==
                                            2) {
                                            ERROR(linenum);
                                        } else if (isExpression(tokenline[linenum], semi22 + 1,
                                                                linelenghts[linenum] - 3) != 1) {
                                            ERROR(linenum);
                                        } else {
                                            loop = 1;
                                        }
                                    }
                                }

                            }
                        }
                    }
                }
            } else if (strcmp(tokenline[linenum][3], "in") == 0) {//not double loop
                int semi1 = finder(":", tokenline[linenum], 4, linelenghts[linenum]);//finds the first colon
                if (semi1 == -1) {
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], 4, semi1 - 1) == 2) {
                    ERROR(linenum);
                } else if (isExpression(tokenline[linenum], 4, semi1 - 1) != 1) {
                    ERROR(linenum);
                } else {
                    int semi2 = finder(":", tokenline[linenum], semi1 + 1,
                                       linelenghts[linenum]);//finds the second colon
                    if (semi2 == -1) {
                        ERROR(linenum);
                    }
                    if (isExpression(tokenline[linenum], semi1 + 1, semi2 - 1) == 2) {
                        ERROR(linenum);
                    } else if (isExpression(tokenline[linenum], semi1 + 1, semi2 - 1) != 1) {
                        ERROR(linenum);
                    } else {
                        if (isExpression(tokenline[linenum], semi2 + 1, linelenghts[linenum] - 3) == 2) {
                            ERROR(linenum);
                        } else if (isExpression(tokenline[linenum], semi2 + 1, linelenghts[linenum] - 3) != 1) {
                            ERROR(linenum);
                        } else {
                            loop = 1;
                        }
                    }
                }

            } else {
                ERROR(linenum);
            }
            linenum++;
            continue;
        }
        if (strcmp(&linelist[0], "print") == 0) {//print statement
            if (isExpression(tokenline[linenum], 2, linelenghts[linenum] - 2) == -1) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][1], "(") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][linelenghts[linenum] - 1], ")") != 0) {
                ERROR(linenum);
            }
            if (gettype(tokenline[linenum][2]) == 0) {
                ERROR(linenum);
            }
            if (linelenghts[linenum] != 4) {//the id is a matrix/vector element with braces like A[x,y]
                if (strcmp("[", tokenline[linenum][3]) == 0) {//checks the braces
                    if (strcmp("]", tokenline[linenum][linelenghts[linenum] - 2]) != 0) {
                        return -1;
                    }
                    if (gettype(tokenline[linenum][2]) == 3) {//vector
                        if (isExpression(tokenline[linenum], 4, linelenghts[linenum] - 3) == 1) {
                            if (linelenghts[linenum] - 3 == 4) {
                                if (isnumber(tokenline[linenum][4]) == 1) {
                                    if (atoi(tokenline[linenum][4]) >
                                        getdimensionsx(tokenline[linenum][2])) {//checks the dimension
                                        ERROR(linenum);
                                    }
                                    if (atoi(tokenline[linenum][4]) == 0) {//checks the dimension
                                        ERROR(linenum);
                                    }
                                }
                            }
                        } else {
                            ERROR(linenum);
                        }
                    }
                    if (gettype(tokenline[linenum][2]) == 2) {//matrix
                        int commaindex = finder(",", tokenline[linenum], 4,
                                                linelenghts[linenum] - 3);//finds the comma between x and y indices
                        int error = -1;
                        while (isExpression(tokenline[linenum], 4, commaindex - 1) !=
                               1) {//looks for a comma until it finds one with an expression on the left
                            commaindex = finder(",", tokenline[linenum], commaindex + 1, linelenghts[linenum] - 3);
                            if (commaindex == -1) {
                                error = 1;
                                break;
                            }
                        }
                        if (error == 1) {
                            ERROR(linenum);
                        }
                        if (isExpression(tokenline[linenum], commaindex + 1, linelenghts[linenum] - 3) !=
                            1) {//checks the right side of the comma
                            ERROR(linenum);
                        }
                        if (strcmp(tokenline[linenum][5], ",") == 0) {
                            if (isnumber(tokenline[linenum][4]) ==
                                1) {//if the comma is in the 5th index and the left side of the comma is a number it compares it to the x dimension of the matrix
                                if (atoi(tokenline[linenum][4]) > getdimensionsx(tokenline[linenum][2])) {
                                    ERROR(linenum);
                                }
                                if (atoi(tokenline[linenum][4]) == 0) {
                                    ERROR(linenum);
                                }
                            }
                        }
                        if (strcmp(tokenline[linenum][linelenghts[linenum] - 4], ",") == 0) {
                            if (isnumber(tokenline[linenum][linelenghts[linenum] - 3]) ==
                                1) {//if the comma is in the minus third index(like "...,a]" ) and the right side of the comma is a number then it campares it to the y dimension of the matrix
                                if (atoi(tokenline[linenum][linelenghts[linenum] - 3]) >
                                    getdimensionsy(tokenline[linenum][2])) {
                                    ERROR(linenum);
                                }
                                if (atoi(tokenline[linenum][linelenghts[linenum] - 3]) == 0) {
                                    ERROR(linenum);
                                }
                            }
                        }
                    }

                }
            }
            linenum++;
            continue;
        }
        if (strcmp(&linelist[0], "printsep") == 0) {//printsep statement, just checks the location every token
            if (linelenghts[linenum] != 3) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][1], "(") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][2], ")") != 0) {
                ERROR(linenum);
            }
            if (strcmp(tokenline[linenum][3], "\n") != 0) {
                ERROR(linenum);
            }
            linenum++;
            continue;
        }
        if (strcmp(tokenline[linenum][1], "=") ==
            0) {// if there is an equal sign in the first index it means it is a scalar or matrix assignment and not a matrix index assignment
            if (id_finder(tokenline[linenum][0]) != -1) {//checks whether the variable is declared
                if (isExpression(tokenline[linenum], 2, linelenghts[linenum] - 1) ==
                    -1) {// checks the right side of the equal sign
                    ERROR(linenum);
                } else if (isExpression(tokenline[linenum], 2, linelenghts[linenum] - 1) ==
                           1) {// checks the type of the right side of the equal sign
                    if (gettype(tokenline[linenum][0]) != 1) {
                        ERROR(linenum);
                    }
                } else {
                    if (gettype(tokenline[linenum][0]) == 1) {
                        ERROR(linenum);
                    }
                }
            }
            if (strcmp(tokenline[linenum][2], "{") ==
                0) {//if there is "{" in the second index that means it is a matrix/vector definition
                if (strcmp(tokenline[linenum][linelenghts[linenum] - 1], "}") != 0) {//checks for the "}"
                    ERROR(linenum);
                }
                if (linelenghts[linenum] - 4 != getdimensionsx(tokenline[linenum][0]) * getdimensionsy(
                        tokenline[linenum][0])) {//compares the number of numbers in braces with the dimensions
                    ERROR(linenum);
                }
                for (int i = 3; i < linelenghts[linenum] - 2; i++) {//checks whether they are all real numbers or not
                    if (isnumber(tokenline[linenum][i]) == 0) {
                        ERROR(linenum);
                    }
                }
                linenum++;
                continue;
            }
            if (gettype(tokenline[linenum][0]) !=
                1) {//if the left side is not scalar it compares the matrix dimensions of the right and the left sides
                if (MatrixSizeCheck(tokenline[linenum], 2, linelenghts[linenum] - 1)[0] == -1) {
                    ERROR(linenum);
                }
                int size[2];
                size[0] = MatrixSizeCheck(tokenline[linenum], 2, linelenghts[linenum] - 1)[0];
                size[1] = MatrixSizeCheck(tokenline[linenum], 2, linelenghts[linenum] - 1)[1];
                if (getdimensionsx(tokenline[linenum][0]) != size[0] ||
                    getdimensionsy(tokenline[linenum][0]) != size[1]) {
                    ERROR(linenum);
                }
            }
            linenum++;
            continue;
        }
        if (finder("=", tokenline[linenum], 0, linelenghts[linenum] - 1) !=
            -1) {//if there is an equal sign but its index is not 2 then it may be a matrix/vector element assignment
            int equalpos = finder("=", tokenline[linenum], 0, linelenghts[linenum]);
            if (finder("=", tokenline[linenum], equalpos + 1, linelenghts[linenum] - 1) !=
                -1) {//there can be at most one equal sign
                ERROR(linenum);
            }
            if (gettype(tokenline[linenum][0]) == 2) {//it is a matrix element assignment
                if (strcmp(tokenline[linenum][1], "[") != 0) {//checks the braces
                    ERROR(linenum);
                }
                if (strcmp(tokenline[linenum][equalpos - 1], "]") != 0) {
                    ERROR(linenum);
                }
                int commaindex = finder(",", tokenline[linenum], 0, equalpos - 1);//finds the comma
                int error = -1;
                while (isExpression(tokenline[linenum], 2, commaindex - 1) !=
                       1) {// keeps looking for a comma until it finds a valid one
                    commaindex = finder(",", tokenline[linenum], commaindex + 1, equalpos - 1);
                    if (commaindex == -1) {
                        error = 1;
                        break;
                    }
                }
                if (error == 1) {
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], 2, commaindex - 1) != 1) {//checks the right side of the comma
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], commaindex + 1, equalpos - 2) !=
                    1) {//checks the left side of the comma
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], equalpos + 1, linelenghts[linenum] - 1) !=
                    1) {//checks the left side of the equal sign
                    ERROR(linenum);
                }
                isExpression(tokenline[linenum], 0,
                             equalpos - 1);//this part makes the left side printable to the output
            } else if (gettype(tokenline[linenum][0]) == 3) {//it is a vector element assignment
                if (strcmp(tokenline[linenum][1], "[") != 0) {//checks the braces
                    ERROR(linenum);
                }
                if (strcmp(tokenline[linenum][equalpos - 1], "]") != 0) {
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], 2, equalpos - 2) != 1) {//checks the inside of the braces
                    ERROR(linenum);
                }
                if (isExpression(tokenline[linenum], equalpos + 1, linelenghts[linenum] - 1) !=
                    1) {// checks the left side of the equal sign
                    ERROR(linenum);
                }
                isExpression(tokenline[linenum], 0,
                             equalpos - 1);//this part makes the left side printable to the output
            } else {
                ERROR(linenum);
            }
            linenum++;
            continue;
        }
        ERROR(linenum);
        linenum++;
    }
    fclose(fpc);

    //All checks are finished. The rest is printing.
    char * o;//creates the output file by just changing the .mat to .c
    o = malloc(sizeof(char) * 256);
    sprintf(o,"");
    strcat(o,argv[1]);
    o[strlen(argv[1]) -3] = 'c';
    o[strlen(argv[1]) -2] = '\0';
    FILE * out = fopen(o,"w");
    initilize(out);
    int doublefor = 0;//to make sure there is no nestes loops
    int expcount = 0;
    for(int i = 0; i <= processlinenum; i++ ){
        if(strcmp(processedtokenline[i][0], "\n") == 0){//empty line
            continue;
        }
        if(strcmp(processedtokenline[i][0],  "scalar") == 0){//scalar declaration
            fprintf(out,"double ");
            fprintf(out,processedtokenline[i][1]);
            fprintf(out,";\n");
            fprintf(out,processedtokenline[i][1]);
            fprintf(out," = 0;\n");
            continue;
        }

        else if(strcmp(processedtokenline[i][0],  "matrix") == 0){//matrix declaration
            fprintf(out,"double %s[%d];\n",processedtokenline[i][1], getdimensionsx(processedtokenline[i][1]) *getdimensionsy(processedtokenline[i][1]) );
            fprintf(out,"pushlenght(%s,%d,%d);\n",processedtokenline[i][1],getdimensionsx(processedtokenline[i][1]),getdimensionsy(processedtokenline[i][1]));
            fprintf(out,"for(int ihopenottaken = 0; ihopenottaken < %d; ihopenottaken++){\n",getdimensionsx(processedtokenline[i][1])*getdimensionsy(processedtokenline[i][1]));
            fprintf(out,processedtokenline[i][1]);
            fprintf(out,"[ihopenottaken] = 0;\n");
            fprintf(out,"}\n");
            continue;
        }
        else if(strcmp(processedtokenline[i][0],  "vector") == 0){//vector declaration
            fprintf(out,"double %s[%d];\n",processedtokenline[i][1], getdimensionsx(processedtokenline[i][1]) *getdimensionsy(processedtokenline[i][1]) );
            fprintf(out,"pushlenght(%s,%d,%d);\n",processedtokenline[i][1],getdimensionsx(processedtokenline[i][1]),getdimensionsy(processedtokenline[i][1]));
            fprintf(out,"for(int ihopenottaken = 0; ihopenottaken < %d; ihopenottaken++){\n",getdimensionsx(processedtokenline[i][1])*getdimensionsy(processedtokenline[i][1]));
            fprintf(out,processedtokenline[i][1]);
            fprintf(out,"[ihopenottaken] = 0;\n");
            fprintf(out,"}\n");
            continue;
        }
        else if(strcmp(processedtokenline[i][0],  "#") == 0){//comment line
            continue;
        }

        else if(strcmp(processedtokenline[i][0],  "for") == 0){//for loop
            int a = 0;
            int count = 0;//colon count
            while(finder(":",processedtokenline[i],a+1,linelenghts[i]-1) != -1){//checks whether it is nested or not
                a = finder(":",processedtokenline[i],a+1,linelenghts[i]-1);
                count++;
            }
            if(count == 2){//not nested
                char * tempid;//creates temporary strings to print
                tempid = malloc(sizeof(char) * 100);
                sprintf(tempid,"");
                for(int k = 2; k < finder("in",processedtokenline[i],2,linelenghts[i] - 1);k++){//takes the identifier
                    strcat(tempid,processedtokenline[i][k]);
                }
                char * tempexpr1;//creates temporary strings to print
                tempexpr1 = malloc(sizeof(char) * 100);
                sprintf(tempexpr1,"");
                int tempsemi1 = finder(":",processedtokenline[i],2,linelenghts[i] - 1);
                for(int k = finder("in",processedtokenline[i],2,linelenghts[i] - 1)+1; k < tempsemi1;k++){//takes the expression1
                    strcat(tempexpr1,processedtokenline[i][k]);
                }
                char * tempexpr2;//creates temporary strings to print
                tempexpr2 = malloc(sizeof(char) * 100);
                sprintf(tempexpr2,"");
                int tempsemi2 = finder(":",processedtokenline[i],tempsemi1+1,linelenghts[i] - 1);
                for(int k = tempsemi1+1; k < tempsemi2;k++){//takes the expression2
                    strcat(tempexpr2,processedtokenline[i][k]);
                }
                char * tempexpr3;//creates temporary strings to print
                tempexpr3 = malloc(sizeof(char) * 100);
                sprintf(tempexpr3,"");
                for(int k = tempsemi2+1; k < linelenghts[i]-2;k++){//takes the expression3
                    strcat(tempexpr3,processedtokenline[i][k]);
                }
                fprintf(out,"for(int %s = %s; %s <= %s; %s += %s){\n",tempid,tempexpr1,tempid,tempexpr2,tempid,tempexpr3);//prints the final string
            }
            else if(count == 4){
                doublefor = 1;
                char * tempid1;//creates temporary strings to print
                tempid1 = malloc(sizeof(char) * 100);
                sprintf(tempid1,"");
                for(int k = 2; k < finder(",",processedtokenline[i],2,linelenghts[i] - 1);k++){//takes the first identifier
                    strcat(tempid1,processedtokenline[i][k]);
                }
                char * tempid2;//creates temporary strings to print
                tempid2 = malloc(sizeof(char) * 100);
                sprintf(tempid2,"");
                for(int k = finder(",",processedtokenline[i],2,linelenghts[i]-1) +1 ; k < finder("in",processedtokenline[i],2,linelenghts[i] - 1);k++){//takes the second identifier
                    strcat(tempid2,processedtokenline[i][k]);
                }
                char * tempexpr1;//creates temporary strings to print
                tempexpr1 = malloc(sizeof(char) * 100);
                sprintf(tempexpr1,"");
                int tempsemi1 = finder(":",processedtokenline[i],2,linelenghts[i] - 1);
                for(int k = finder("in",processedtokenline[i],2,linelenghts[i] - 1)+1; k < tempsemi1;k++){//takes the first expression
                    strcat(tempexpr1,processedtokenline[i][k]);
                }
                char * tempexpr2;//creates temporary strings to print
                tempexpr2 = malloc(sizeof(char) * 100);
                sprintf(tempexpr2,"");
                int tempsemi2 = finder(":",processedtokenline[i],tempsemi1+1,linelenghts[i] - 1);
                for(int k = tempsemi1+1; k < tempsemi2;k++){//takes the second expression
                    strcat(tempexpr2,processedtokenline[i][k]);
                }
                char * tempexpr3;//creates temporary strings to print
                tempexpr3 = malloc(sizeof(char) * 100);
                sprintf(tempexpr3,"");
                int tempcomma = finder(",",processedtokenline[i],tempsemi2+1,linelenghts[i] - 1);
                for(int k = tempsemi2+1; k < tempcomma  ;k++){//takes the third expression
                    strcat(tempexpr3,processedtokenline[i][k]);
                }
                char * tempexpr4;//creates temporary strings to print
                tempexpr4 = malloc(sizeof(char) * 100);
                sprintf(tempexpr4,"");
                int tempsemi3 = finder(":",processedtokenline[i],tempsemi2+1,linelenghts[i] - 1);
                for(int k = tempcomma+1; k < tempsemi3 ;k++){//takes the fourth expression
                    strcat(tempexpr4,processedtokenline[i][k]);
                }
                char * tempexpr5;//creates temporary strings to print
                tempexpr5 = malloc(sizeof(char) * 100);
                sprintf(tempexpr5,"");
                int tempsemi4 = finder(":",processedtokenline[i],tempsemi3+1,linelenghts[i] - 1);
                for(int k = tempsemi3+1; k < tempsemi4 ;k++){//takes the fifth expression
                    strcat(tempexpr5,processedtokenline[i][k]);
                }
                char * tempexpr6;//creates temporary strings to print
                tempexpr6 = malloc(sizeof(char) * 100);
                sprintf(tempexpr6,"");
                for(int k = tempsemi4+1; k < linelenghts[i]-2 ;k++){//takes the sixth expression
                    strcat(tempexpr6,processedtokenline[i][k]);
                }
                fprintf(out,"for(int %s = %s; %s <= %s; %s += %s){\n",tempid1,tempexpr1,tempid1,tempexpr2,tempid1,tempexpr3);//prints the final strings
                fprintf(out,"\tfor(int %s = %s; %s <= %s; %s += %s){\n",tempid2,tempexpr4,tempid2,tempexpr5,tempid2,tempexpr6);
            }
        }
        else if(strcmp(processedtokenline[i][0],  "}") == 0 && doublefor == 1){//if the loop was nested it prints 2 "}"
            fprintf(out,"}}\n");
            doublefor = 0;
        }
        else if(strcmp(processedtokenline[i][1],  "=") == 0){//assignment statement
            if(strcmp(processedtokenline[i][2],  "{") == 0){//matrix/vector definition
                if(strcmp(processedtokenline[i][2],  "{") == 0){
                    for(int a = 0; a < getdimensionsx(processedtokenline[i][0]) *getdimensionsy(processedtokenline[i][0]); a++){//assigns each element separately
                        fprintf(out,"%s[%d] = %s;\n",processedtokenline[i][0],a,processedtokenline[i][a+3]);
                    }
                }
                continue;
            }
            if(gettype(processedtokenline[i][0]) == 1){//scalar assignment line, prints as it is(mostly)
                fprintf(out,"%s = ",processedtokenline[i][0]);
                for(int k = 2; k < linelenghts[i]; k++ ){
                    if(strcmp(processedtokenline[i][k]," ") == 0){
                        continue;
                    }
                    fprintf(out,"%s ",processedtokenline[i][k]);
                }
                fprintf(out,";\n");
            }
            else{//matrix/vector assignment
                int temp = getdimensionsx(processedtokenline[i][0])* getdimensionsy(processedtokenline[i][0]);
                fprintf(out,"double * ihopeitsfine%d;\n",expcount);//creates an intermediate matrix to store the value
                for(int k = 3; k < linelenghts[i]-1; k++){
                    strcat(processedtokenline[i][2],processedtokenline[i][k]);
                }
                fprintf(out,"ihopeitsfine%d = %s;\n",expcount,processedtokenline[i][2]);
                fprintf(out,"for(int ihopenottaken = 0; ihopenottaken < %d; ihopenottaken++){\n",temp);//prints a for loop to assign each value one by one from the intermediate matrix to the assigned matrix
                fprintf(out,"%s[ihopenottaken] = ihopeitsfine%d[ihopenottaken];\n",processedtokenline[i][0],expcount);
                fprintf(out,"}\n");
                expcount++;
            }
        }
        else if(finder("=",processedtokenline[i],0,linelenghts[i] -1) != -1){//matrix/vector element assignment, prints as it is(with the necessary changes)
            for(int k = 0; k < linelenghts[i]; k++ ){
                if(strcmp(processedtokenline[i][k]," ") == 0){
                    continue;
                }
                fprintf(out,"%s ",processedtokenline[i][k]);
            }
            fprintf(out,";\n");
        }
        else if(strcmp(processedtokenline[i][0],  "print") == 0){//print statement
            int temp = processlinenum;
            processlinenum = i;
            isExpression(tokenline[i],2,linelenghts[i]-2);//makes it printable
            processlinenum = temp;
            if(gettype(processedtokenline[i][2]) != 1){//matrix/vector
                if(strcmp(processedtokenline[i][3],  "[") == 0){//element printing
                    for(int k = 0; k < linelenghts[i]; k++){
                        fprintf(out,"%s",processedtokenline[i][k]);
                    }
                    fprintf(out,";\n");
                }
                else{//prints a for loop to print all elements of matrices/vectors
                    fprintf(out,"for(int ihopenottaken = 0; ihopenottaken< %d;ihopenottaken++){\n",getdimensionsx(processedtokenline[i][2])* getdimensionsy(processedtokenline[i][2]));
                    fprintf(out,"if(ihopenottaken %% %d == 0 && ihopenottaken !=0){\n", getdimensionsy(processedtokenline[i][2]));
                    fprintf(out,"printf(\"\\n\");}\n");
                    fprintf(out,"if(fabs((int)%s[ihopenottaken] - %s[ihopenottaken])   <0.0000009){\n",processedtokenline[i][2],processedtokenline[i][2]);
                    fprintf(out,"printf(\"%%d \",(int)%s[ihopenottaken]);}\n",processedtokenline[i][2]);
                    fprintf(out,"else{\n");
                    fprintf(out,"printf(\"%%f \",%s[ihopenottaken]);}\n",processedtokenline[i][2]);
                    fprintf(out,"}\n");
                    fprintf(out,"printf(\"\\n\");\n");
                }
            }
            else{//scalar printing
                for(int k = 0; k < linelenghts[i]; k++){
                    fprintf(out,"%s",processedtokenline[i][k]);
                }
                fprintf(out,";\n");
            }
        }
        else if(strcmp(processedtokenline[i][0],  "printsep") == 0){//printsep statement, nothing exciting happens here
            for(int k = 0; k < linelenghts[i]; k++){
                fprintf(out,"%s",processedtokenline[i][k]);
            }
            fprintf(out,";\n");
        }
        else {
            fprintf(out,processedtokenline[i][0]);
            fprintf(out," \n");
        }
    }

    fprintf(out,"}");

    fclose(out);

}

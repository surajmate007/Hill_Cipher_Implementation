#include<iostream>
#include <climits>
#include<fstream>
#define N 100

using namespace std;

int GCD(int a, int b){
    if(b==0){
        return a;
    }
    return GCD(b, a%b);
}

int mulInverse(int det){
    det = det % 26;
    if(det < 0){
        det = det + 26;
    }
    int mul_inverse = 0;
    for (int mi = 1; mi < 26; mi++){
        if ((det * mi) % 26 == 1){
            mul_inverse = mi;
        }
    }
    return mul_inverse;
}
   
float IOC(string plainText){
    int freqSum = 0; int N1 = plainText.length();
    float IC;
    int freq[26] = {0};
    for(int i=0; i<plainText.length(); i++){
        freq[plainText.at(i) - 97]++;
    }
    for(int i=0; i<26; i++){
        freqSum = freqSum + (freq[i] * (freq[i]-1)); 
    }
    float denominator = N1*(N1-1) * 1.0;
    IC = freqSum/denominator;
    return IC;
}

void getCofactor(int A[N][N], int temp[N][N], int p, int q, int n){
    int i = 0, j = 0;
    for (int row = 0; row < n; row++){
        for (int col = 0; col < n; col++){
            if (row != p && col != q){
                temp[i][j++] = A[row][col];
                if (j == n - 1){
                    j = 0;
                    i++;
                }
            }
        }
    }
    return;
}
 
int determinant(int A[N][N], int n){
    int D = 0;
    if (n == 1) return A[0][0];
    int temp[N][N]; 
    int sign = 1; 
    for (int f = 0; f < n; f++)
    {
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return D;
}
 
void adjoint(int A[N][N],int adj[N][N], int n){
    if (n == 1){
        adj[0][0] = 1;
        return;
    }
    int sign = 1; int temp[N][N];
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            getCofactor(A, temp, i, j, n);
            sign = ((i+j)%2==0)? 1: -1;
            adj[j][i] = (sign)*(determinant(temp, n-1));
        }
    }
    return;
}

void multiplyN(int mat1[N][N], int mat2[N][N], int res[N][N], int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<n; k++){
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
            res[i][j] = res[i][j] % 26;
        }
    }
}

string decryptCompleteCipher(string cipher, int keylen, int keyInverse[N][N], string decrypted){
    int p=0; int q=0;
    int cphr [N][N] = {0};
    while(q < cipher.length()){
        for(int i=0; i<keylen; i++){
            for(int j=0; j<keylen; j++){
                if(p < cipher.length()){
                    cphr[i][j] = cipher.at(p);
                    p++;
                }  
            }
        }
        int dcrptMat[N][N] = {0};
        multiplyN(keyInverse, cphr, dcrptMat, keylen);

        for(int i=0; i<keylen; i++){
            for(int j=0; j<keylen; j++){
                decrypted += dcrptMat[i][j] + 97;
            }
        }
        q = q + (keylen * keylen);
    }
    return decrypted;
}

int maxIC(float ICarray[10]){
    float max = 0.0;
    int keyval = 0;
    for(int i=0; i<10; i++){
        if(ICarray[i] > max){
            max = ICarray[i];
            keyval = i;
        }
    }
    return keyval;
}



int main(){
    ifstream file1;
    file1.open("cipher.txt");
    string cipher;
    file1>>cipher;
    file1.close();

    ifstream file2;
    file2.open("message.txt");
    string messagetxt;
    while(file2.eof() == 0){
        getline(file2, messagetxt);
    }
    file2.close();
    float ICarray[10] = {0};
    string message = "";
    for(int i=0; i<messagetxt.size(); i++){
        char ch = messagetxt.at(i);
        if(ch!=' ' && ch!='.' && ch!=',' && ch!='?' && ch!='/' && ch!='(' && ch!=')'){
            ch = tolower(ch);
            message += ch;
        }
    }

    int N1 = cipher.length();
    int keylen = 2;
    float max = 0.0;
    int key_ans = 0;
    float avg = 0;
    int count = 0;

    while(keylen < 11){
        int p=0; int q=0;

        int cphr[N][N] = {0};
        int plain[N][N] = {0};
        int keyMat[N][N] = {0};

        while(p < message.length()){
            for(int i=0; i<keylen; i++){
                for(int j=0; j<keylen; j++){
                    if((q+j+p) < message.length()){
                        cphr[i][j] = cipher.at(q+j+p) - 65;
                        plain[i][j] = message.at(q+j+p) - 97;
                    }
                }
                q+=keylen;
            }
            int det1 = determinant(plain, keylen);
            int gcd1 = GCD(det1, 26);

            if(det1 != 0 && gcd1 == 1){
                int mInverse1 = mulInverse(det1);
                int plainInverse [N][N] = {0};
                adjoint(plain, plainInverse, keylen);
                for(int i=0; i<keylen; i++){
                    for(int j=0; j<keylen; j++){
                        int m = plainInverse[i][j];
                        m = m * mInverse1;
                        m = m % 26;
                        if(m < 0){
                            m = m + 26;
                        }
                        plainInverse[i][j] = m; 
                    }
                }
                multiplyN(cphr, plainInverse, keyMat, keylen);

                // finding key inverse :
                int det2 = determinant(keyMat, keylen);
                int gcd2 = GCD(det2, 26);
                if(det2 != 0 && gcd2 == 1){
                    int mInverse2 = mulInverse(det2);
                    int keyInverse[N][N] = {0};
                    adjoint(keyMat, keyInverse, keylen);
                    for(int i=0; i<keylen; i++){
                        for(int j=0; j<keylen; j++){
                            int m = keyInverse[i][j];
                            m = m * mInverse2;
                            m = m % 26;
                            if(m < 0){
                                m = m + 26;
                            }
                            keyInverse[i][j] = m; 
                        }
                    }

                    string decrypted = decryptCompleteCipher(cipher, keylen, keyInverse, "");

                    float calculatedIOC = IOC(decrypted);
                    ICarray[keylen] = calculatedIOC;
                    p = message.length();

                }
                else{
                    p++;
                }
            }
            else{
                p++;
            }
        }
        keylen++;
    }

    int maxICvalue = maxIC(ICarray);
    cout<<"The key used for encryption is : ";
    cout<<maxICvalue<<endl; 
    return 0;
}

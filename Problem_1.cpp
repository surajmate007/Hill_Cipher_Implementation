#include<iostream>
#include<bits/stdc++.h>
using namespace std;
#define N 100


void multiply(int mat1[N][N], int mat2[N][1], int res[N][1], int n){
    for(int i=0; i<n; i++){
        res[i][0] = 0;
        for(int k=0; k<n; k++){
            res[i][0] += mat1[i][k] * mat2[k][0];
        }
        res[i][0] = res[i][0] % 26;
    }
}

int takeKeyInput(int key[N][N]){
    ifstream file1;
    file1.open("key.txt");
    string keytxt;
    getline(file1, keytxt);
    file1.close();

    string keyInput = "";
    for(int i=0; i<keytxt.length(); i++){
        if(keytxt.at(i) != ' '){
            keyInput += keytxt.at(i);
        }
    }
    int num = (int) keyInput.at(0) - 48;
    int p = 1;
    for(int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            key[i][j] = (int) keyInput.at(p) - 48;
            p++;
        }
    }
    return num;
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

int main(){
    
    int key[N][N] = {0};
    int n = takeKeyInput(key);

    ifstream file2;
    file2.open("message.txt");
    string messagetxt;
    while(file2.eof() == 0){
        getline(file2, messagetxt);
    }
    file2.close();

    string message = "";
    for(int i=0; i<messagetxt.size(); i++){
        char ch = messagetxt.at(i);
        if(ch!=' ' && ch!='.' && ch!=',' && ch!='?' && ch!='/' && ch!='(' && ch!=')'){
            ch = tolower(ch);
            message += ch;
        }
    }

    while(message.length() % n != 0){
        message += "x";
    }

    int p=0;
    int q=0;
    int msg [N][1] = {0}; 
    int cphr[message.length()] = {0};
    int res1[N][1];
    while(p != message.size()){
        int i = 0;
        for(i=0; i<n && p<message.size(); i++){
            msg[i][0] = message.at(p) - 97;
            p++;
        }
        multiply(key, msg, res1, n);

        for(int i=0; i<n; i++){
            int m = res1[i][0];
            res1[i][0] = m % 26;
            cphr[q] = 65 + res1[i][0];
            q++;
        } 
    }

    ofstream file3;
    file3.open("cipher.txt");
    string cipher = "";
    for(int i=0; i<message.size(); i++){
        cipher += cphr[i];
    }
    file3<<cipher;
    file3.close();    

    int det = determinant(key, n);
    int mInverse = mulInverse(det);

    if(mInverse == 0){
        cout<<"Invalid key ! \nDecryption not possible"<<endl;
        return -1;
    }

    int keyInverse [N][N] = {0};
    adjoint(key, keyInverse, n);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int m = keyInverse[i][j];
            m = m * mInverse;
            m = m % 26;
            if(m < 0){
                m = m + 26;
            }
            keyInverse[i][j] = m;   
        }
    }

    char dcrpt[message.length()] = {0};
    int res2[n][1] = {0};
    int cphrMat[n][1] = {0};
    p = 0; q = 0;

    while(p != cipher.size()){
        for(int i=0; i<n && p<cipher.size(); i++){
            cphrMat[i][0] = cipher.at(p) - 65;
            p++;
        }

        multiply(keyInverse, cphrMat, res2, n);

        for(int i=0; i<n; i++){
            int m = res2[i][0];
            m = m % 26;
            if(m < 0){
                m = m + 26;
            }
            res2[i][0] = m;
            dcrpt[q] = 97 + res2[i][0];
            q++;
        }
    }       

    ofstream file4;
    file4.open("decrypted.txt");
    string decrypted = "";
    for(int i=0; i<message.length(); i++){
        decrypted += dcrpt[i];
    }
    // cout<<decrypted<<endl;
    cout<<"Successfully Executed !!!"<<endl;
    file4<<decrypted;
    file4.close();
    
    return 0;
}

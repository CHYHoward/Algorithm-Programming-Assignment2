#include <fstream>
#include <iostream>
#include <chrono>
#include <cstdint>
using namespace std;
using namespace std::chrono;

const uint32_t GETNUM = 262143;
const uint32_t GETSTATUS = 786432;
const uint32_t TAKE = 262144;
const uint32_t NOTAKE = 524288;

uint32_t mps(uint32_t i, uint32_t j, uint32_t* data, uint32_t** table);
void output(uint32_t i, uint32_t j, uint32_t* data, uint32_t** table, ostream& out);

int main(int argc, char* argv[])
{
    //////////////// read input file ////////////////////
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);

    // input size (number of nodes)
    uint32_t N;
    fin >> N;

    // input connection relationships
    uint32_t a, b;
    uint32_t* data = new uint32_t [N];

    for (uint32_t i = 0; i < N / 2; i++)
    {
        fin >> a >> b;
        data[a] = b;
        data[b] = a;
    }
    
    // Check if all data was input
    fin >> a;
    if (a != 0)
    {
        cout << "ERROR: WRONG INPUT";
        return 1;
    }

    ///////////////// read input file complete ////////////////

    // table init
    uint32_t** table = new uint32_t* [N];
    for (uint32_t i = 0; i < N; i++)
    {
        table[i] = new uint32_t [N - i];
        for (uint32_t j = 0; j < N - i; j++)
            table[i][j] = 0;
    }

    // auto start = high_resolution_clock::now();
    fout << mps(0, N - 1, data, table) << endl;
    // auto stop = high_resolution_clock::now();

    // auto duration = duration_cast<microseconds>(stop - start);
    // cout << duration.count() << endl;
    output(0, N-1, data, table, fout);
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N - i; j++)
    //     {
    //         cout << (table[i][j]) << "\t";
    //     }
    //     cout << endl;
    // }

    fin.close();
    fout.close();
    return 0;
}

uint32_t mps(uint32_t i, uint32_t j, uint32_t* data, uint32_t** table)
{
    if (table[i][j - i] != 0)
    {   
        return (table[i][j - i] & GETNUM);
    }
    if (i == j) 
    {
        return 0;
    }

    uint32_t connect_with_j = data[j];

    if (connect_with_j > j || connect_with_j < i)
    {
        table[i][j - i] = NOTAKE + mps(i, j - 1, data, table);
    }

    else if (connect_with_j == i)
    {
        table[i][j - i] = TAKE + mps(i + 1, j - 1, data, table) + 1;
    }

    else
    {
        
        uint32_t takeit = mps(i, connect_with_j - 1, data, table) + mps(connect_with_j + 1, j - 1, data, table) + 1;
        uint32_t notake = mps(i, j - 1, data, table);

        if (takeit >= notake)
            table[i][j - i] = TAKE + takeit;
        else
            table[i][j - i] = NOTAKE + notake;
    }
    return (table[i][j - i] & GETNUM);
}

void output(uint32_t i, uint32_t j, uint32_t* data, uint32_t** table, ostream& out)
{
    if (j <= i) return;
    if ((table[i][j - i] & GETSTATUS) == NOTAKE)
    {
        output(i, j - 1, data, table, out);
        return;
    }
    else
    {
        if (data[j] == 0)   
        {
            out << data[j] << " " << j << endl;
            return;
        }
        output(i, data[j] - 1, data, table, out);
        out << data[j] << " " << j << endl;
        output(data[j] + 1, j - 1, data, table, out);
        return;
    }
}
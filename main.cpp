#include <iostream>
#include <cassert>
#include <fstream>
using namespace std;
//12800 - 254300 = 12630
//41600 - 49800 = 8200
const int max_noord = 49800;
const int max_zuid = 41600;
const int max_oost =  25430;
const int max_west = 12800;
typedef int coord[5];
typedef bool hintdec[10];
typedef char coordhint[10];
typedef coordhint hints[6];
typedef hintdec solutionb[6];
typedef coordhint DB[126300];
typedef int paterndatabase[115975];
struct coordsi
{
    int northing;
    int easting;
};
struct coords
{
    coord n;
    coord e;
};
struct line
{
    coords from;
    coords to;
    double angle;
    double length;
};

struct Areai
{
    coordsi bottumleft;
    coordsi topright;
};

struct deelgebiedhint
{
    bool online;
    char patern[10];
    char paternfiles[10];
    hintdec posibilities[10];
    Areai deelgebied;
};
struct hintall
{
    char patern[60];
    bool online[6];
    Areai gld;
    hintdec posibilities[10];
    deelgebiedhint deelgebieden[6];
};
void fill_array(char array[], char content, int length)
{
    for (int i = 0; i < length; i++)
        array[i] = content;
}
void fill_array(int array[], int content, int length)
{
    for (int i = 0; i < length; i++)
        array[i] = content;
}
void fill_array(bool array[], bool content, int length)
{
    for (int i = 0; i < length; i++)
        array[i] = content;
}
void open_file(ifstream& file, char newpatern[10])
{
    char filenamedb[] = {'.','\\','d','a','t','a','b','a','s','e','\\'};
    char postfix [] = {'.','t','x','t','\0'};
    char filename[26];
    for (int i = 0; i < 26;i++)
        if (i < 11)
            filename[i] = filenamedb[i];
        else if (i < 21)
            filename[i] = newpatern[i-11];
        else
            filename [i] = postfix[i-21];
    file.open(filename);
}
coordsi ll_to_coordsi(long long i)
{
    coordsi a;
    a.northing = i % 100000;
    a.easting = (i - a.northing) / 100000;
    return a;
}
coords coordsi_tocoords(coordsi ci)
{
    coords cs;
    int temp = ci.easting;
    for (int i = 0; i < 5;i++)
    {
        cs.e[4-i] = temp % 10;
        temp = (temp - (temp % 10)) / 10;
    }
    temp = ci.northing;
    for (int i = 0; i < 5;i++)
    {
        cs.n[4-i] = temp % 10;
        temp = (temp - (temp % 10)) / 10;
    }
    return cs;
}
bool read_pattern(/*string patern,*/ char newpatern[10], char oldpatern[10], Areai a,hintdec hd[10])
{
    long long co;
    coordsi ci;
    for (int i = 0; i <10;i++)
        fill_array(hd[i],false,10);
    ifstream file;
    hintdec used;
    fill_array(used, false , 10);
    int index = 0;
    open_file(file, newpatern);
    char c;
    while (file)
    {
        file >> co;
        file.get(c);
        ci = ll_to_coordsi(co);
        bool b1 =  (ci.easting > a.bottumleft.easting && ci.easting < a.topright.easting);
        bool b2 =  (ci.northing > a.bottumleft.northing && ci.northing < a.topright.northing);
        bool b3 =  (ci.easting == 0 && ci.northing == 0);
        if (b1&&b2)
        {
            coords cs = coordsi_tocoords(ci);
            for (int i = 0; i < 10;i++)
            {
                int index2 = oldpatern[i]-'a';
                used[index2] = true;
                if (i<5)
                    hd[index2][cs.e[i]] = true;
                else
                    hd[index2][cs.n[i-5]] = true;
            }
        }

    }
    for (int i = 0; i<10;i++)
    {
        if (!used[i])
            fill_array(hd[i],true,10);
    }
    file.close();
}
deelgebiedhint create_deelgebiedhint(char patern[10], bool online, Areai a)
{
    deelgebiedhint dh;
    dh.deelgebied = a;
    dh.online = online;
    if (!dh.online)
    {
        fill_array(dh.patern,'a',10);
        fill_array(dh.paternfiles, 'a', 10);
    }
    else
    {
        for (int i = 0; i< 10;i++)
            dh.patern[i] = patern [i];
        char temp_patern[10];
        char highest[10];
        for (int i = 0; i <10;i++)
            temp_patern[i] = patern [i];
        char highestc = 'a';
        if (temp_patern[0] != 'a')
        {
            char temp_char = temp_patern[0];
            for (int i = 0; i < 10;i++)
                if (temp_patern[i] == temp_char)
                    temp_patern[i] = 'a';
                else if (temp_patern[i] == 'a')
                    temp_patern[i] = temp_char;
        }
        for (int i = 1; i < 10; i++)
        {
            char temp_char = temp_patern[i];
            if (temp_char > highestc +1)
            {
                highestc++;
                for (int j = 0; j < 10;j++)
                if (temp_patern[j] == temp_char)
                    temp_patern[j] = highestc;
                else if (temp_patern[j] == highestc)
                    temp_patern[j] = temp_char;
            } else if (temp_char == highestc + 1)
                highestc++;
        }
        for (int i = 0; i<10;i++)
            dh.paternfiles[i] = temp_patern[i];
        read_pattern(dh.paternfiles,dh.patern,dh.deelgebied,dh.posibilities);
    }
}
hintall create_hintall(deelgebiedhint d[6],Areai a)
{
    hintall ha;
    for (int i = 0; i < 6; i++)
        ha.deelgebieden[i] = d[i];
    ha.gld = a;
    for (int i = 0; i < 6; i++)
    {
        ha.online[i] = ha.deelgebieden[i].online;
        for (int j = 0; j < 10;j++)
            ha.patern[i+j] = ha.deelgebieden[i].patern[j];
    }
}
void output_all_coords(ofstream& file, string filename, int fromn,int ton, int frome, int toe)
{
    file.open(filename.c_str());
    for (int i = fromn; i < ton; i++)
        for (int j = frome; j < toe; j++)
            file << j << '-' << i << endl;
    file.close();
}

bool find_higest (char patern[], char highest[], int length)
{
    highest[0] = patern[0];
    bool isOk = true;
    for (int i = 1; i < length; i++)
    {
        if (patern[i] > highest[i - 1] + 1)
            isOk = false;
        highest[i] = highest [i - 1];
        if (patern[i] == highest[i - 1] + 1)
            highest [i] = patern[i];
    }
    return isOk;
}

void wait (string label)
{
    cout << label;
    string i;
    cin >>i;

}
void find_coords(char patern[], int length)
{
    char filename[16 + length];
    char folder[11] = {'.','\\','d','a','t','a','b','a','s','e','\\'};
    char postfix[5] = {'.','t','x','t','\0'};
    for (int i = 0; i < 16 + length; i ++)
    {
        if (i < 11)
            filename [i] = folder [i];
        else if (i < 11 + length)
            filename [i] = patern [i-11];
        else
            filename [i] = postfix [i-11 - length];
    }
    cout << filename << '\t';
    ofstream file;
    file.open (filename);
    char highest[length];
    find_higest(patern,highest,length);
    int amm_comb = highest[length-1] - highest[0] + 1;
    int combinations[amm_comb];
    for (int i = 0; i < length; i++)
        combinations[i] = i;
    int index  = amm_comb - 1;
    bool done = false;
    bool uniquen = true;
    bool maxindex = true;
    int n = 0;
    while (!done)
    {
        uniquen = true;
        maxindex = true;
        if (!(combinations[index] < 10))
        {
            combinations[index--] = 0;
            if (index == -1)
                done = true;
        }else
        {
            for (int i = 0; i < index; i++)
                if (combinations[i] == combinations[index])
                    uniquen = false;
            if (uniquen)
            {
                if (index == amm_comb - 1)
                {
                    for (int i = 0; i < length;i++)
                    {
                    int varindex = patern[i] - patern[0];
                    file << combinations[varindex];
                    }
                    file << endl;
                    n++;
                }
                else
                {
                    index++;
                    maxindex = false;
                }
            }
    }
        if (!(done)  && maxindex)
            combinations[index]++;
    }
    file.close();
    cout << n <<endl;
}
void output_all_patterns(ofstream& file, string filename,int length, char start)
{
    file.open(filename.c_str());
    char patern[length + 1];
    char higest[length + 1];
    higest[length] = '\0';
    patern[length] = '\0';
    fill_array(patern, start,length);
    int index = length - 1;
    int n = 0;
    while (patern[0] == start)
    {
        index = length - 1;
        while (find_higest(patern,higest,length))
        {
            file << patern << endl;
            find_coords(patern,length);
            patern[index]++;
            n++;
        }
        do
        {
            patern [index] = start;
            patern [--index]++;
        }
        while (!find_higest(patern,higest,length));
    }
    cout << n << endl;
    file.close();
}
void fill_database()
{
    ofstream file;
    string filename = ".\\database\\paterns.txt";
    char start = 'a';
    int length = 10;
    output_all_patterns(file,filename,length,start);
}

bool in_Areai(Areai a, coordsi c)
{

   bool b =  (c.easting > a.bottumleft.easting && c.easting < a.topright.easting) && (c.northing > a.bottumleft.northing && c.northing < a.topright.northing);
   //cout << a.bottumleft.easting <<'-'<<a.bottumleft.northing<<'-'<< a.topright.easting <<'-'<<a.topright.northing<<'-'<< c.easting <<'-'<<c.northing<<'-'<<(c.easting >= a.bottumleft.easting)<<'-'<<(c.easting <= a.topright.easting) <<'-'<<(c.northing >= a.bottumleft.northing)<<'-'<<(c.northing <= a.topright.northing)<<'-'<<b<<endl;
  // cout << b <<endl;
    return b || (c.easting == 0 && c.northing == 0);
}



void show_in_area(string patern, char newpatern[10], char oldpatern[10], Areai a,hintdec hd[10])
{
    long long co;
    coordsi ci;
    for (int i = 0; i <10;i++)
        fill_array(hd[i],10,true);
    ifstream file;
    int i = 0;
    open_file(file, newpatern);
    char c;
    while (file)
    {
        file >> co;
        file.get(c);
        ci = ll_to_coordsi(co);
        bool b1 =  (ci.easting > a.bottumleft.easting && ci.easting < a.topright.easting);
        bool b2 =  (ci.northing > a.bottumleft.northing && ci.northing < a.topright.northing);
        bool b3 =  (ci.easting == 0 && ci.northing == 0);
        if (b1&&b2)
        {
            coords c = coordsi_tocoords(ci);
            for (int i = 0; i < 10;i++)
            {


            }
        }

    }
    file.close();
}
void make_deelgebieden(Areai deelgebieden[7])
{
    deelgebieden[0].bottumleft.northing = 46500;
    deelgebieden[0].bottumleft.easting = 15550;
    deelgebieden[0].topright.northing = 50400;
    deelgebieden[0].topright.easting = 19850;

    deelgebieden[1].bottumleft.northing = 44300;
    deelgebieden[1].bottumleft.easting = 19030;
    deelgebieden[1].topright.northing = 50400;
    deelgebieden[1].topright.easting = 21580;

    deelgebieden[2].bottumleft.northing = 42700;
    deelgebieden[2].bottumleft.easting = 19574;
    deelgebieden[2].topright.northing = 47300;
    deelgebieden[2].topright.easting = 25400;

    deelgebieden[3].bottumleft.northing = max_zuid;
    deelgebieden[3].bottumleft.easting = max_west;
    deelgebieden[3].topright.northing = max_noord;
    deelgebieden[3].topright.easting = max_oost;

    deelgebieden[4].bottumleft.northing = max_zuid;
    deelgebieden[4].bottumleft.easting = max_west;
    deelgebieden[4].topright.northing = max_noord;
    deelgebieden[4].topright.easting = max_oost;

    deelgebieden[5].bottumleft.northing = max_zuid;
    deelgebieden[5].bottumleft.easting = max_west;
    deelgebieden[5].topright.northing = max_noord;
    deelgebieden[5].topright.easting = max_oost;

    deelgebieden[6].bottumleft.northing = max_zuid;
    deelgebieden[6].bottumleft.easting = max_west;
    deelgebieden[6].topright.northing = max_noord;
    deelgebieden[6].topright.easting = max_oost;
}
bool contchar(char c, char s[], int length)
{
    bool cont = false;
    for (int i = 0; i < length && s[i] != '\0';i++)
        if (c==s[i])
            cont = true;
    return cont;
}
int main()
{
    char cs[6];
    cout << "online";
    cin >> cs;
    bool online[6];
    fill_array(online,false,6);
    for (int i = 0 ; i < 6; i++)
    {
        online[i] = contchar('a' + i, cs,6);
    }
    //show_in_area("abcabdefgh",a);
    //show_in_area("abcdeddeaf",a);
    //show_in_area("aabcacbdec",a);
    //show_in_area("abbccdecfg",a);
    //show_in_area("abcdcecfbg",a);
    //show_in_area("abcdabefee",a);
   // string s = "hallo";
    //char c[5] = ;
    //char c[5] = s.c_str();
   // for (int i = 0;i<5;i++)
     //   cout << s << ' ' << /*c<<*/s.c_str() <<' '<<endl;
    //fill_database();
    cout << "Hello world!" << endl;
    return 0;
}

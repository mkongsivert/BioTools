/**
 * \file bitvector.cpp
 *
 * \authors Mackenzie Kong-Sivert
 *
 * \brief Implements the bit_vector, rank_support, select_support, and 
 * FM_text classes.
 */

#include "bitvector.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <math.h>
#include <chrono>

bit_vector::bit_vector() :
        size_{0}, num_bytes_{0}
{
    bytes_ = NULL;
}

bit_vector::bit_vector(std::string seq)
{
    num_bytes_ = ceil(float(seq.length())/8.0);
    bytes_ = new uint8_t[num_bytes_];
    uint64_t counter = 0;
    uint8_t curr = 0;
    for (uint64_t i=0; i<=seq.length(); ++i)
    {
        curr = curr*2 + int(seq[i]);
        if (i%8==7)
        {
            bytes_[(i-7)/8] = curr;
            curr = 0;
        }
    }
    size_ = seq.length();

    // Add padding if necessary
    if (size_%8 != 0)
    {
        uint64_t pad = 8 - (counter%8);
        curr = curr* pow(2, pad);
        bytes_[num_bytes_-1] = curr;
    } 
}

void bit_vector::save(std::string& fname)
{
    std::ofstream myfile(fname, std::ofstream::out);
    std::string bitstr;
    for (uint64_t i = 0; i < num_bytes_; ++i)
    {
        bitstr += dec_to_str(bytes_[i]);
    }
    myfile << "Bitstring: " << bitstr << std::endl;
    myfile << "Size: " << std::to_string(size_) << std::endl;
    myfile.close();
}

void bit_vector::load(std::string& fname)
{
    std::string line;
    std::string contents;
    std::ifstream myfile (fname);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            contents += line;
        }
        myfile.close();
        std::string line_copy = "";
        for (uint64_t i = 0; i < contents.size(); ++i)
        {
            if (contents[i] == '0' || contents[i] == '1')
            {
                line_copy.push_back(contents[i]);
            }
        }
        *this = bit_vector(line_copy);
    }

    else std::cout << "Unable to open file"; 
}

uint64_t bit_vector::size()
{
    return size_;
}

uint64_t bit_vector::num_bytes()
{
    return num_bytes_;
}

std::string bit_vector::dec_to_str(uint8_t n)
{
    if (n <= 1)
    {
        return std::to_string(n);
    }
    else
    {
        return dec_to_str(n/2) + std::to_string(n%2);
    }
}

void bit_vector::print()
{
    std::string bitstr;
    for (uint64_t i = 0; i < num_bytes_; ++i)
    {
        bitstr += dec_to_str(bytes_[i]);
    }
    std::cout << "Bitstring: " << bitstr << std::endl;
    std::cout << "Size: " << std::to_string(size_) << std::endl;
}

std::string rank_support::dec_to_str(uint8_t n)
{
    if (n <= 1)
    {
        return std::to_string(n);
    }
    else
    {
        return dec_to_str(n/2) + std::to_string(n%2);
    }
}

uint64_t rank_support::rank_helper(uint64_t n, uint64_t i, uint64_t b)
{
    uint64_t msb = (n >= pow(2,b-1) ? 1 : 0);
    if (i == 0)
    {
        return msb;
    }
    else
    {
        return msb + rank_helper(n-(msb*pow(2,b-1)), i-1, b-1);
    }
}

rank_support::rank_support()
{
    size_ = 0;
    bits_ = bit_vector();
    s_ = 0;
    b_ = 0;
}

rank_support::rank_support(bit_vector bits) :
        bits_{bits}, size_{bits.size()}
{
    s_ = pow(log2(size_), 2)/2;
    b_ = log2(size_)/2;

    uint64_t sCount = 0;
    uint64_t bCount = 0;
    uint64_t *sArray = new uint64_t[size_/s_];
    uint64_t *bArray = new uint64_t[size_/b_];
    for (uint64_t i = 0; i < bits.num_bytes(); ++i)
    {
        std::string this_byte = dec_to_str(bits.bytes_[i]);
        for (uint64_t j = 0; j < 8; ++j)
        {
            uint64_t index = 8*i+j;
            if (index%b_ == 0)
            {
                bArray[index/b_] = bCount;
            }
            if (index%s_ == 0)
            {
                sArray[index/s_] = sCount;
                bCount = 0; // reset b entries for every s entry
            }
            sCount += this_byte[j]=='1' ? 1 : 0;
            bCount += this_byte[j]=='1' ? 1 : 0;
        }
    }
    Rs_ = sArray;
    Rb_ = bArray;

    // Build matrix separately
    Rp_ = new uint64_t*[int(pow(2,b_))];
    for (uint64_t i = 0; i < pow(2,b_); ++i)
    {
        Rp_[i] = new uint64_t[b_];
        for (uint64_t j = 0; j < b_; ++j)
        {
            Rp_[i][j] = rank_helper(i,j,b_);
        }
    }
}

uint64_t rank_support::get_bit_i(uint64_t i)
{
    uint8_t byte = bits_.bytes_[int(float(i)/8.0)];
    uint8_t pos = i%8;
    return int(float(byte)/pow(2,7-pos)) % 2;
}

uint64_t rank_support::rank1(uint64_t i)
{
    uint8_t n = 0; // bitstring type
    uint64_t j = i%b_; // index within bitstring
    for (uint64_t k = i-j; k < i-j+b_; ++k)
    {
        n = 2*n + get_bit_i(k);
    }
    // TODO: Maybe clean this up?
    return *(Rs_+(i/s_)) + *(Rb_+(i/b_)) + Rp_[n][j];
}

uint64_t rank_support::rank0(uint64_t i)
{
    return i - rank1(i);
}

void rank_support::print()
{
    bits_.print();

    std::cout << "\nR_p:";
    for (uint64_t a = 0; a < pow(2,b_); ++a)
    {
        for (uint64_t b = 0; b < b_; ++b)
        {
            std::cout << Rp_[a][b];
            std::cout << (b < b_-1 ? ", " : "");
        }
        std::cout << std::endl;
        std::cout << (a < pow(2,b_)-1 ? "    " : "");
    }

    std::cout << "R_s: ";

    for (uint64_t i = 0; i <= (size_/s_); ++i)
    {
        std::cout << *(Rs_+i) << ", ";
    }

    std::cout << "\nR_b: ";
    for (uint64_t j = 0; j <= (size_/b_); ++j)
    {
        std::cout << *(Rb_+j) << ", ";
    }
    std::cout << "\ns: " << s_ << "\nb: " << b_ << std::endl;
    std::cout << "\nOverhead:" + std::to_string(overhead()) << std::endl;
}

uint64_t rank_support::overhead()
{
    return sizeof(this)*8; // sizeof returns size in bytes
}

void rank_support::save(std::string& fname)
{
    std::ofstream myfile(fname, std::ofstream::out);
    bits_.save(fname);

    myfile << "\nR_p:";
    for (uint64_t a = 0; a < pow(2,b_); ++a)
    {
        for (uint64_t b = 0; b < b_; ++b)
        {
            myfile << Rp_[a][b];
            myfile << (b < b_-1 ? ", " : "");
        }
        myfile << std::endl;
        myfile << (a < pow(2,b_)-1 ? "    " : "");
    }

    myfile << "R_s: ";

    for (uint64_t i = 0; i <= (size_/s_); ++i)
    {
        myfile << *(Rs_+i) << ", ";
    }

    myfile << "\nR_b: ";
    for (uint64_t j = 0; j <= (size_/b_); ++j)
    {
        myfile << *(Rb_+j) << ", ";
    }
    myfile << "\ns: " << s_ << "\nb: " << b_ << std::endl;
    myfile << "\nOverhead:" + std::to_string(overhead()) << std::endl;
    myfile.close();
}

void rank_support::load(std::string& fname)
{
    bit_vector bits;
    bits.load(fname);
    *this = rank_support(bits);
}

uint64_t rank_support::size()
{
    return bits_.size();
}

select_support::select_support(rank_support r_supp) :
    r_supp_{r_supp}
{
    // nothing to do here
}

uint64_t select_support::select1_help(uint64_t i, uint64_t n, uint64_t m)
{
    uint64_t mid = (n+m)/2;
    uint64_t midRank = r_supp_.rank1(mid);
    
    if (midRank == i)
    {
        return mid;
    }
    else if ((m-n) < 1)
    {
        std::cout << "ERROR: desired index not found";
        return 0;
    }
    else if (midRank < i)
    {
        return select1_help(i, n, mid);
    }
    else
    {
        return select1_help(i, mid, m);
    }
}

uint64_t select_support::select1(uint64_t i)
{
    return select1_help(i, 0, r_supp_.size());
}

uint64_t select_support::select0_help(uint64_t i, uint64_t n, uint64_t m)
{
    uint64_t mid = (n+m)/2;
    uint64_t midRank = r_supp_.rank0(mid);
    
    if (midRank == i)
    {
        return mid;
    }
    else if ((m-n) < 1)
    {
        std::cout << "ERROR: desired index not found";
        return 0;
    }
    else if (midRank < i)
    {
        return select0_help(i, n, mid);
    }
    else
    {
        return select0_help(i, mid, m);
    }
}

uint64_t select_support::select0(uint64_t i)
{
    return select0_help(i, 0, r_supp_.size());
}

uint64_t select_support::overhead()
{
    return sizeof(this)*8;
}

void select_support::print()
{
    r_supp_.print();
}

void select_support::save(std::string& fname)
{
    r_supp_.save(fname);
}

void select_support::load(std::string& fname)
{
    rank_support r_supp;
    r_supp.load(fname);
    *this = select_support(r_supp);
}

ichar::ichar(char c, uint64_t i) :
    c_{c}, i_{i}
{
    // nothing to do here
}

FM_text::FM_text() :
    size_{0}, text_{""}
{
    // nothing to do here
}

uint64_t FM_text::ind(char c)
{
    switch(c)
    {
        case 'a' :
            return 0;
            break;
        case 'c' :
            return 1;
            break;
        case 'g' :
            return 2;
            break;
        case 't' :
            return 3;
            break;
        default:
            std::cout << "Error: tried to query " << c << std::endl;
            return 4;
            break;
    }
}

FM_text::FM_text(std::string text) :
    size_{text.size()}, text_{text}
{
    // NOTE: this is specifically tailored to gene sequences because I am
    // very tired.
    std::vector<ichar> bwt = BWT(text);
    F_ = &bwt[0];
    L_ = &bwt[size_+1];

    Occ_ = new uint64_t*[(size_/10)+1];
    uint64_t row[4] = {0};
    for (uint64_t i = 0; i <= size_; ++i)
    {
        if ((L_+i)->c_ != '$')
        {
            row[ind((L_+i)->c_)]+=1;
        }
        if (i%10 == 0)
        {
            Occ_[i/10] = new uint64_t[4];
            for (uint8_t j = 0; j < 4; ++j)
            {
                Occ_[i/10][j] = row[j];
            }
        }
    }

    for (uint8_t j = 0; j < 4; ++j)
    {
        tally_[j] = row[j];
    }
}

uint64_t FM_text::size()
{
    return size_;
}

void FM_text::print()
{
    std::cout << text_ << std::endl;
}

void FM_text::print_lstring(std::vector<ichar> text)
{
    for (auto itr = text.begin(); itr != text.end(); ++itr)
    {
        std::cout << itr->c_ << itr->i_;
    }
    std::cout << std::endl;
}

bool FM_text::prefix_less_than(std::string str0, std::string str1)
{
    if (str0 == "")
    {
        return true;
    }
    else if (str1 == "")
    {
        return false;
    }
    else if (str0.front()==str1.front())
    {
        return prefix_less_than(str0.substr(1), str1.substr(1));
    }
    else
    {
        return (str0.front()<str1.front());
    }
}

std::vector<ichar> FM_text::label_indices(std::string T)
{
    std::vector<ichar> output;
    //assuming case-insensitive alphanumeric characters (can alter later)
    uint64_t alpha[37] = {0};
    for (auto itr = T.begin(); itr != T.end(); ++itr)
    {
        if (*itr == '$')
        {
            output.push_back(ichar('$',0));
        }
        else
        {
            uint64_t ind = ((int)*itr <= 57 ? (int)*itr - 48 : (int)*itr - 87);
            output.push_back(ichar(*itr, alpha[ind]));
            ++alpha[ind];
        }
    }
    return output;
}

std::vector<ichar> FM_text::BWT(std::string text)
{
    std::vector<std::string> rotations;
    std::string curr = text+"$";
    rotations.push_back(curr);
    for (uint64_t i = 0; i < size_; ++i)
    {
        //rotate string
        curr.push_back(curr.front());
        curr.erase(curr.begin());
        //place string and indices in appropriate position
        for (auto itr = rotations.begin(); itr != rotations.end(); ++itr)
        {
            if (prefix_less_than(curr, *itr))
            {   
                rotations.insert(itr, curr);
                break;
            }
            else if (++itr == rotations.end())
            {
                rotations.push_back(curr);
                break;
            }
            --itr;
        }
    }
    // extract F and L
    std::string Fstring;
    for (auto itr = rotations.begin(); itr != rotations.end(); ++itr)
    {
        Fstring.push_back((*itr)[0]);
    }
    std::vector<ichar> output = label_indices(Fstring);
    std::string Lstring;
    for (auto itr = rotations.begin(); itr != rotations.end(); ++itr)
    {
        Lstring.push_back((*itr)[size_]);
    }
    std::vector<ichar> out_temp = label_indices(Lstring);
    output.insert(output.end(), out_temp.begin(), out_temp.end());
    return output;
}

std::vector<uint64_t> FM_text::query(std::string pre)
{
    if (pre.size() == 1)
    {
        uint64_t first = 1;
        for (uint8_t i = 0; i < ind(pre[0]); ++i)
        {
            first += tally_[i];
        }
        //find first and last indices with this character
        uint64_t last = first + tally_[ind(pre[0])];
        std::vector<uint64_t> out;
        out.push_back(first);
        out.push_back(last);
        return out;
    }
    else
    {
        std::vector<uint64_t> interval = query(pre.substr(1));
        // find transition from 1st to 2nd char
        uint8_t chai = ind(pre[0]);
        uint64_t first = Occ_[(interval.front()/10)][chai];
        uint64_t check0 = interval.front() - interval.front()%10 + 1;

        for (uint64_t i = check0; i <= interval.front(); ++i)
        {
            std::cout << "running down underground " << L_[1].c_ << std::endl;
            first += ((L_+i)->c_==pre[0] ? 1 : 0);
            std::cout << "lenny bruce is not afraid" << std::endl;
        }
        std::cout << "birds snakes an aeroplane" << std::endl;
        uint64_t last = Occ_[interval.back()/10][chai];
        std::cout << "that's great it starts with an earthquake" << std::endl;
        uint8_t check1 = interval.back() - interval.back()%10 + 1;
        for (uint8_t i = check1; i <= interval.back(); ++i)
        {
            last += (L_[i].c_==pre[0] ? 1 : 0);
        }

        // find corresponding index in F_
        for (uint8_t i = 0; i < chai; ++i)
        {
            first += tally_[i];
        }
        last += 1;
        for (uint8_t i = 0; i < chai; ++i)
        {
            last += tally_[i];
        }
        std::cout << pre << std::endl;
        std::vector<uint64_t> out;
        out.push_back(first);
        out.push_back(last);
        return out;
    }
}

void FM_text::FM_index()
{
    // retrieve F and L
    
}

int main()
{
    std::string line;
    std::string contents;
    std::ifstream myfile ("test0.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            contents += line;
        }
        myfile.close();
        std::string line_copy = "";
        for (uint64_t i = 0; i < contents.size(); ++i)
        {
            line_copy.push_back(std::tolower(contents[i]));
        }
        std::cout << "Ready" << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        FM_text test = FM_text(line_copy);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::vector<uint64_t> ind = test.query("aaa");
        auto t3 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
        std::cout << "Size: " << test.size() << std::endl;
        std::cout << "Duration 1: " << duration1 << std::endl;
        std::cout << "Duration 2: " << duration2 << std::endl;
    }

    else std::cout << "Unable to open file"; 
    return 0;
}

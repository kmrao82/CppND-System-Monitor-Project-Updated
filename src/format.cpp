#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hh, mm, ss, rem;
    string hhs,mms,sss;
    hh= seconds/3600;
    rem= seconds%3600;
    mm= rem/60;
    ss=rem%60;
    

    string returnString = " ";
    if(hh <10){
    hhs= "0"+ std::to_string(hh);
    }
    else
    {
        hhs= std::to_string(hh);
    }
   if(mm <10){
    mms= "0"+ std::to_string(mm);
    }
    else
    {
        mms= std::to_string(mm);
    }
    if(ss <10){
    sss= "0"+ std::to_string(ss);
    }
    else
    {
        sss= std::to_string(ss);
    }

    returnString= hhs+":"+ mms+":"+ sss;

     return returnString;
}
#include <iostream>
#include <fstream>
#include "string"
#include <vector>
#include "curl/curl.h"
#include <cassert>
#include <cstring>
#include <iterator>
#include <string>
#include <cstdio>
#include <ctime>
#include <zip.h>
#include "basen.hpp"
#include <set>
#include <map>
#include<sstream>
#include<exception>
#include<profile.h>
#include <future>
#include<functional>
#include<whoisds.h>

class Config
{
private:


public:
    Config() {}

};


using namespace std;
//Буфер для курла !
string curlBuffer;
size_t curlWriteFunc(char *data,size_t size,size_t nmemb, string *buffer){
    size_t result = 0;

    if(buffer != NULL){
            buffer->append(data, size * nmemb);
            result = size * nmemb;
    }
    return result;
}




class Site {
private:

    string url;
    string suspkeyword;
    int points = 0;
    string rating="Not Cheked";
    bool reachable = false;
public:
  //constructor
 explicit Site (const string &u,const unordered_map<string,int> &k ):url(u){
      string toclean = u.c_str();
      string cleaned;
      copy_if(toclean.begin(), toclean.end(),
         back_inserter(cleaned),
         [] (char c) { return c != '\r' && c != '\n'; });
       url = cleaned;

      asignKeyword(k);

      //Не знаю нужна ли здесь эта функци проверки
     // checkSite();
  }
    //Конструктор без проверки на кейворды
    explicit Site (const string &u){
        string toclean = u.c_str();
        string cleaned;
        copy_if(toclean.begin(),toclean.end(),back_inserter(cleaned),
                [](char c){return c!='\r' && c != '\n';});
        url=cleaned;

    }
//Доступ к переменым класа
inline  string Url () const {return url ;}
inline string Keyword () const {return suspkeyword;}
inline  int Points ()  const {return points;}
inline  string Rating() const {return rating;}
inline   bool Reachable()  const {return reachable;}


//void reportInFile();
void inline setUrl(const string &y)
{
     url = y;

}
void asignKeyword(const unordered_map<string,int> &Keywords);
//void rankPoint();
void LevensteinAnalizer(const vector<string> &bankurls) ;

//void asignRating;

void checkSite();

};
  // перегрузка операторов  для класса

bool operator < (const  Site & lhs, const  Site & rhs){
    if(lhs.Points()<rhs.Points()){
        return true;
    }
    else {
        return false;
    }
}

ostream & operator << (ostream & out, const Site &site){

        out<<site.Url()<<","<<site.Keyword()<<","<<site.Points()<<","<<site.Rating()<<","<<site.Reachable();
        return out;
    }
// функция чтения файлов в вектор
void readinvect (const string &filename, vector<string> & V){

    ifstream file (filename);
    string line;
        while (getline(file,line)){
            V.push_back(move(line));
        }




}
// функция чтения файла в словарь ключевых слов с значениями
void LoadKeyWords (const string & filename, unordered_map<string,int> &Keywords){


    ifstream file(filename);
    string line;
        while(getline(file,line)){
            string tmp;
            int p;
            stringstream ss(line);
            ss>>tmp;
            ss.ignore(1);
            ss>>p;
            Keywords[move(tmp)]=move(p);

        }

}

//функция поиска ключевых слов в адресе + начисление баллов
void Site::asignKeyword (const unordered_map<string,int> &Keywords){

    for( auto &keyword:Keywords){

        //(stringsearch(url,keyword.first))

        if(url.find(keyword.first) != url.npos){
            suspkeyword +=keyword.first.c_str();
            suspkeyword +=" ";
            points+=keyword.second;

        }
    }
}


  void Site::checkSite(){

    const char * adress = url.c_str();
    //буфер для сохраненияы ошибок
    char curlErrorBuffer[CURL_ERROR_SIZE];

    //Инициализируем Курл
    CURL *curl = curl_easy_init();

    // проверяем инициализацию
    if (curl){

        // Задаем опции курлу
        // Буфер для ошибок с вишеописаной функцией
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorBuffer );
        // задаем адресс

        curl_easy_setopt(curl, CURLOPT_URL, adress);
        //задаем юзер агент
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0");
        //задаем реферал
         curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
        // переходим по локейшену задоному в заголовку
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
       //куки ???
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        // НЕ ПРОВЕРЯТЬ СЕРТИФИКАТ
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);







        //указываем что полученые даные нужно записать  в буфер используя функцию

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);

       //выполянем сам запрос
        cout<<"Cheking "<< url<<endl;
        CURLcode curlResult = curl_easy_perform(curl);
       // закрываем сеанс

        curl_easy_cleanup(curl);

        //проверяем резултат

        if (curlResult == CURLE_OK){
            reachable=1;

            //здесь должна быть проверка результата
           // cout<< curlBuffer;
            // возвращаем стрингу
            //return curlBuffer ;

        }
        else {
            reachable=0;
            cout << "FuckUp("<<curlResult<<"); LOL"<< curlErrorBuffer << endl;
        }
    }

}


// Поиск дистанции левинштейна


  size_t LevenshteinDistance(const string &source, const string &target) {
      int checklimit  = 0; // стопор до трех
      int count = 0;

      if (source.size()<=4 ) {
          return 10;
      }

      if(source.size() <= 6){
          checklimit = 1;
      }
      else {
          checklimit = 2;
      }

      if(target.find(source)!=target.npos && source.size()>5){
          return 1;
      } // убрать

      const size_t min_size = source.size(), max_size = target.size();
      vector<size_t> lev_dist(min_size + 1);

      for (size_t i = 0; i <= min_size; ++i) {
          lev_dist[i] = i;
      }

      for (size_t j = 1; j <= max_size; ++j) {
          size_t previous_diagonal = lev_dist[0], previous_diagonal_save;
          ++lev_dist[0];
          if(count>checklimit){
              return 10;
          }
          for (size_t i = 1; i <= min_size; ++i) {

              previous_diagonal_save = lev_dist[i];
              if (source[i - 1] == target[j - 1]) {
                  lev_dist[i] = previous_diagonal;
                  count++;
              } else {
                  lev_dist[i] = min(min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;


              }
              previous_diagonal = previous_diagonal_save;
          }
      }

      return lev_dist[min_size];
  }


// Другая имплементация
 /* unsigned int LevenshteinDistance(const std::string& s1, const std::string& s2)
  {
      const std::size_t len1 = s1.size(), len2 = s2.size();
      std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

      d[0][0] = 0;
      for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
      for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

      for(unsigned int i = 1; i <= len1; ++i)
          for(unsigned int j = 1; j <= len2; ++j)
                        // note that std::min({arg1, arg2, arg3}) works only in C++11,
                        // for C++98 use std::min(std::min(arg1, arg2), arg3)
                        d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
      return d[len1][len2];
  }*/


  void LevensteinAnalizer(const vector<string> &bankurls, const string & url ){
      auto point_it = find(url.begin(),url.end(),'.');
      string clean_source (url.begin(),point_it);


      for ( const string &x:bankurls){

          if (LevenshteinDistance(x,clean_source)<=2 ){
              //заменить код и функцию под класс

              cout  << "Lev < 3 mask : "<< x << "   source :   "<< url<< endl;

              break;
          }
     }
  }

  void MultythreadLevenstein (const vector<string> & banks ,const vector<string> &listofurl){
      vector<future<void>> threads;

      for (auto &x:listofurl){

         threads.push_back(async(LevensteinAnalizer,ref(banks),ref(x)));

      }
  }

// нужно привести эту хуергу в порядок


int main()
{

    unordered_map<string,int> keywords;
    LoadKeyWords("keywords",keywords);
    getWhoisds();
    vector<string> sitelist ;
    readinvect("domain-names.txt",sitelist);
    vector<string> levi;
    readinvect("banks",levi);
    vector<Site> result;
    vector<string> pseudoclean;
    cout<<"Start Cheking sites "<<endl;
{       LOG_DURATION("Site Intital cheker")
        result.reserve(sitelist.size());
        pseudoclean.reserve(sitelist.size());
        for (   string   &x:sitelist){
                Site site(x,keywords);
                if(site.Points()>0){
                result.push_back(site);

                }
                else {
                    pseudoclean.push_back(x);
                }

        }
}
    result.shrink_to_fit();
    pseudoclean.shrink_to_fit();
    sort(result.rbegin(),result.rend());

        for ( auto &x:result){
            cout<<x<<endl;
        }
    cout<<"Cheking suspiscius sites "<<endl;
    cout<<result.size()<<endl;

{
    LOG_DURATION("Rest of masive Levenstein check")
    cout<<"Size of remain vector "<<pseudoclean.size()<<endl;
    // здесь вызвать многопоточного левинштейна !!!
     MultythreadLevenstein(levi,pseudoclean);


}

    // Гавно-функцию чексайт нада перепиливать целиком и полностью
        for (auto & site:result){
            site.checkSite();
        }
    cout<<"Succesful end"<<endl;

}


void getWhoisds()
{

        // определяем переменые для прощета даты
        time_t rawtime;
        tm* timeinfo;
        char buffer [11];
       // инициализируем текущее вермя
        time(&rawtime);
        // считаем вчерашнюю дату и записываем в буфер
        rawtime = rawtime - 24*3600;
        timeinfo = localtime(&rawtime);
        strftime(buffer,80,"%Y-%m-%d",timeinfo);
       //переносим  дату и нициализируем переменые для формировки урл
        string filename (buffer);
        filename = filename+".zip";
        //кодируем название файла в бейс65
        string encoded;
        bn::encode_b64(filename.begin(), filename.end(), back_inserter(encoded)),ostream_iterator<char>(cout,"");
        //формируем урл
        string fetchurl = "https://whoisds.com//whois-database/newly-registered-domains/"
                            +encoded+"=/nrd";
        cout << fetchurl<<endl;
        // скачиваем архив !
        CURL *curl;
         FILE *fp;

         const char *urm = fetchurl.c_str();
         char curlErrorBuffer[CURL_ERROR_SIZE];
         char outfilename[FILENAME_MAX] = "temp.zip";
         curl = curl_easy_init();

         if (curl)
         {

             cout<<"Creating file !!"<<endl;;
             fp = fopen(outfilename,"wb");
             curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorBuffer );
             curl_easy_setopt(curl, CURLOPT_URL, urm);
             curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
             curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

             curl_easy_perform(curl);

            curl_easy_cleanup(curl);
           //Нужно допилить проверку

           fclose(fp);

         }
         else {
             cout<<"Fucking erro !"<<endl;
         }
         int err = 0;
         // открываем архив
            zip *z = zip_open("temp.zip", 0, &err);

            //поиск нужного файла
            const char *name = "domain-names.txt";
            struct zip_stat st;
            zip_stat_init(&st);
            zip_stat(z, name, 0, &st);

            //создаем пространство в памяти для содержимого архива
            char *contents = new char[st.size];

            //считываем и записываем содержимое файла
            if (zip_file *f = zip_fopen(z, "domain-names.txt", 0)){
            zip_fread(f, contents, st.size);
            ofstream("domain-names.txt").write(contents, st.size);


            zip_fclose(f);

            //закрываем архив
            zip_close(z);
            }
            else {
                cout<<"zalupa"<<endl;
            }

}

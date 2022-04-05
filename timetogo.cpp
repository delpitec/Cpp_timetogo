#include <iostream>
#include <fstream> 
#include <vector> 
#include <sstream>
#include <ctime>

using namespace std;

class ConfigurationTime{
    public:

        ConfigurationTime(){
            SetJobBeginHour(0);
            SetJobBeginMinute(0);
            SetLunchDurationHour(0);
            SetLunchDurationMinute(0);
            SetFilePath("Configuration");
            SetJobDurationHour(0);
            SetJobDurationMinute(0);

            SetLastConfigDay(0);
            SetLastConfigMonth(0);
            SetLastConfigYear(0);
        }

        ConfigurationTime(string filePath){
            SetFilePath(filePath);
            ReadFileAll();
        }

        bool FileConfigurationTimeIsUpdated(void){
            // Get current time
            std::tm tmFileTime {0};
            std::time_t timeNow = std::time(0);
            std::tm* tmTimeNow = localtime(&timeNow);
            
            if (tmTimeNow->tm_mday != GetLastConfigDay() ||
                tmTimeNow->tm_mon + 1  != GetLastConfigMonth() || 
                tmTimeNow->tm_year - 100 != GetLastConfigYear() ){
                    return false;
            }
            else{
                return true;
            }
        }

        void SetJobBeginFromKeyboard(void){
            int getHour, getMin;
            string input = "";
            
            do{
                getHour = getMin = 0;
                
                cout << "Digite horário de entrada: ";
                getline(cin,input);
                for (char& c : input){
                    if (!isdigit(c)){
                        c = ' ';
                    }
                }
                stringstream ss(input);     
                ss >> getHour >> getMin;

            }while ((getHour < 0 || getHour > 23) || (getMin < 0 || getMin > 59));

            SetJobBeginHour(getHour);
            SetJobBeginMinute(getMin);
        }

        void ReadFileAll(void){
            std::vector<std::string> line;
            std::string input;

            fstream configFile;
            configFile.open(_filePath, std::fstream::in);
    
            // store config numeric values at vector
            while (std::getline(configFile, input)){
                line.push_back(input);   
            }

            for (unsigned int i = 0; i < line.size(); i++){
                line.at(i).erase(0,22);
            }

            SetJobBeginHour(stoi(line.at(0)));
            SetJobBeginMinute(stoi(line.at(1)));

            SetLunchDurationHour(stoi(line.at(2)));
            SetLunchDurationMinute(stoi(line.at(3)));
            SetJobDurationHour(stoi(line.at(4)));
            SetJobDurationMinute(stoi(line.at(5)));

            SetLastConfigDay(stoi(line.at(6)));
            SetLastConfigMonth(stoi(line.at(7)));
            SetLastConfigYear(stoi(line.at(8)));
            configFile.close();
        }

        void WriteFileAll(void){
            std::ofstream configFile;
            configFile.open(_filePath, std::ofstream::out | std::ofstream::trunc);
            configFile << "JOB_BEGIN_HOUR      = " << GetJobBeginHour() << endl
                       << "JOB_BEGIN_MIN       = " << GetJobBeginMinute() << endl
                       << "LUNCH_DURATION_HOUR = " << GetLunchDurationHour() << endl
                       << "LUNCH_DURATION_MIN  = " << GetLunchDurationMinute() << endl
                       << "JOB_DURATION_HOUR   = " << GetJobDurationHour() << endl
                       << "JOB_DURATION_MIN    = " << GetJobDurationMinute() << endl;
            configFile.close();
            WriteFileUpdateLastConfigData();
        }

        void WriteFileUpdateLastConfigData(void){
            std::vector<std::string> line;
            std::string input;
            fstream inputConfigFile;
            fstream outputConfigFile;
              
            // read all Configuration file
            inputConfigFile.open(_filePath, std::fstream::in);
            while (std::getline(inputConfigFile, input)){
                line.push_back(input);   
            }
            inputConfigFile.close();

            // write Configuration file [refresh last day, month and year file config for NOW date]
            outputConfigFile.open(_filePath, std::fstream::out | std::fstream::trunc);
            for (unsigned int i = 0; i < 6 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 
            
            // Get current time
            std::tm tmFileTime = {0};
            std::time_t timeNow = std::time(0);
            std::tm* tmTimeNow = localtime(&timeNow); 
            
            outputConfigFile  << "LAST_CONFIG_DAY     = " << tmTimeNow->tm_mday << endl
                              << "LAST_CONFIG_MON     = " << tmTimeNow->tm_mon + 1 << endl
                              << "LAST_CONFIG_YEAR    = " << tmTimeNow->tm_year - 100 << endl;
            outputConfigFile.close();
        }

        void WriteFileJobBegin(void){
            std::vector<std::string> line;
            std::string input;
            fstream inputConfigFile;
            fstream outputConfigFile;
            
            // read all Configuration file
            inputConfigFile.open(_filePath, std::fstream::in);
            while (std::getline(inputConfigFile, input)){
                line.push_back(input);   
            }
            inputConfigFile.close();

            //rewrite whole file
            outputConfigFile.open(_filePath, std::fstream::out | std::fstream::trunc);
            outputConfigFile  << "JOB_BEGIN_HOUR      = " << GetJobBeginHour() << endl
                              << "JOB_BEGIN_MIN       = " << GetJobBeginMinute() << endl;
            for (unsigned int i = 2; i < 6 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 

            outputConfigFile.close();

            WriteFileUpdateLastConfigData();
        }

        void WriteFileJobDuration(void){
            std::vector<std::string> line;
            std::string input;
            fstream inputConfigFile;
            fstream outputConfigFile;
            
            // read all Configuration file
            inputConfigFile.open(_filePath, std::fstream::in);
            while (std::getline(inputConfigFile, input)){
                line.push_back(input);   
            }
            inputConfigFile.close();

            //rewrite whole file
            outputConfigFile.open(_filePath, std::fstream::out | std::fstream::trunc);
            for (unsigned int i = 0; i < 4 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 
            outputConfigFile  << "JOB_DURATION_HOUR   = " << GetJobDurationHour() << endl
                              << "JOB_DURATION_MIN    = " << GetJobDurationMinute() << endl;
            for (unsigned int i = 6; i < 9 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 
            outputConfigFile.close();
        }

        void WriteFileLunchDuration(void){
            std::vector<std::string> line;
            std::string input;
            fstream inputConfigFile;
            fstream outputConfigFile;
            
            // read all Configuration file
            inputConfigFile.open(_filePath, std::fstream::in);
            while (std::getline(inputConfigFile, input)){
                line.push_back(input);   
            }
            inputConfigFile.close();

            //rewrite whole file
            outputConfigFile.open(_filePath, std::fstream::out | std::fstream::trunc);
            for (unsigned int i = 0; i < 2 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 
            outputConfigFile  << "LUNCH_DURATION_HOUR = " << GetLunchDurationHour() << endl
                              << "LUNCH_DURATION_MIN  = " << GetLunchDurationMinute() << endl;
            for (unsigned int i = 4; i < 9 ; i++){
                outputConfigFile << line.at(i) << endl;
            } 
            outputConfigFile.close();
        }
        
        void SetFilePath(string filePathName) {_filePath = filePathName;}
        void SetJobBeginHour(unsigned int value){_jobBegin.hour = value; }
        void SetJobBeginMinute(unsigned int value){_jobBegin.minute = value; }
        void SetLunchDurationHour(unsigned int value){_lunchDuration.hour = value; }
        void SetLunchDurationMinute(unsigned int value){_lunchDuration.minute = value; }
        void SetJobDurationHour(unsigned int value){_jobDuration.hour = value; }
        void SetJobDurationMinute(unsigned int value){_jobDuration.minute = value; }
        void SetLastConfigDay(unsigned int value){_lastConfig.day = value; }
        void SetLastConfigMonth(unsigned int value){_lastConfig.month = value; }
        void SetLastConfigYear(unsigned int value){_lastConfig.year = value; }
        string GetFilePath(void) { return _filePath;}
        unsigned int GetJobBeginHour(void){ return _jobBegin.hour; }
        unsigned int GetJobBeginMinute(void){ return _jobBegin.minute; }
        unsigned int GetLunchDurationHour(void){ return _lunchDuration.hour; }
        unsigned int GetLunchDurationMinute(void){ return _lunchDuration.minute; }
        unsigned int GetJobDurationHour(void){ return _jobDuration.hour; }
        unsigned int GetJobDurationMinute(void){ return _jobDuration.minute; }
        unsigned int GetLastConfigDay(void){ return _lastConfig.day; }
        unsigned int GetLastConfigMonth(void){ return _lastConfig.month; }
        unsigned int GetLastConfigYear(void){ return _lastConfig.year; }
        unsigned int GetJobRemainingTimeHour(void){return CalculateJobRemainingTime(Hour);}
        unsigned int GetJobRemainingTimeMinute(void){return CalculateJobRemainingTime(Minute);}
        unsigned int GetEndTimeHour(void){return CalculateEndTime(Hour);}
        unsigned int GetEndTimeMinute(void){return CalculateEndTime(Minute);}

    private:
        string _filePath;
        enum TimeAndDateUnit {Hour, Minute, Day, Month, Year};
        struct JOB_BEGIN {unsigned int hour; unsigned int minute;} _jobBegin;
        struct LUNCH_DURATION  {unsigned int hour; unsigned int minute;} _lunchDuration;
        struct JOB_DURATION {unsigned int hour; unsigned int minute;} _jobDuration;
        struct LAST_CONFIG {unsigned int day ; unsigned int month; unsigned int year;} _lastConfig;

        unsigned int CalculateJobRemainingTime(TimeAndDateUnit unit){       
            // Current time            
            std::tm tmFileTime = {0};
            std::time_t timeNow = std::time(0);
            std::tm* tmTimeNow = localtime(&timeNow);

            // End Time
            std::tm tmEnd = *tmTimeNow;
            tmEnd.tm_hour = GetJobBeginHour() + GetLunchDurationHour() + GetJobDurationHour();
            tmEnd.tm_min =  GetJobBeginMinute() + GetLunchDurationMinute() + GetJobDurationMinute();
            
            time_t difftimeSeconds = difftime(mktime(&tmEnd), mktime(tmTimeNow));

            if (unit == Hour){
                return(difftimeSeconds / 3600);
            }
            else{
                return ((difftimeSeconds / 60) - ((difftimeSeconds / 3600) * 60));
            }
        }

        unsigned int CalculateEndTime(TimeAndDateUnit unit){
            std::tm tmEnd = {0};
            tmEnd.tm_hour = GetJobBeginHour() + GetLunchDurationHour() + GetJobDurationHour();
            tmEnd.tm_min =  GetJobBeginMinute() + GetLunchDurationMinute() + GetJobDurationMinute();
            mktime(&tmEnd);
            if (unit == Hour){
                return(tmEnd.tm_hour);
            }
            else{
                return (tmEnd.tm_min);
            }
        }


};

int main(int argc, char* argv[])
{
    ConfigurationTime config = ConfigurationTime("Configuration");

    if (argc > 0){
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "-h") || (arg == "--help")) {
                cout << "Exibir menu de help (em desenvolvimento)" << endl;
                return 0;
            }
            if ((arg == "-j") || (arg == "--jornada")) {
                if ((i + 2) <= argc ){
                    arg = argv[++i];
                    config.SetJobDurationHour(stoi(arg));
                    arg = argv[++i];
                    config.SetJobDurationMinute(stoi(arg));
                    config.WriteFileJobDuration();
                }
                else{
                    cout << "teste jornada - faltam argumentos" << endl;
                }
                return 0;
            }
            if ((arg == "-a") || (arg == "--almoco")) {
                if ((i + 2) <= argc ){
                    arg = argv[++i];
                    config.SetLunchDurationHour(stoi(arg));
                    arg = argv[++i];
                    config.SetLunchDurationMinute(stoi(arg));
                    config.WriteFileLunchDuration();
                }
                else{
                    cout << "teste almoco - faltam argumentos" << endl;
                }
                return 0;
            }
        }
    }

    // check if Configuration read from file is updated
    if (!config.FileConfigurationTimeIsUpdated()){
        config.SetJobBeginFromKeyboard();
        config.WriteFileJobBegin();
    }

    printf("\n####################################\n");
    printf("#                                  #\n"  );
    printf("#  Hora de entrada    : %02dh %02dmin  #\n",config.GetJobBeginHour(), config.GetJobBeginMinute());
    printf("#  Duração do trabalho: %02dh %02dmin  #\n",config.GetJobDurationHour(), config.GetJobDurationMinute());
    printf("#  Duração do almoço  : %02dh %02dmin  #\n",config.GetLunchDurationHour(), config.GetLunchDurationMinute());
    printf("#                                  #\n"  );
    printf("#  Hora de saida      : %02dh %02dmin  #\n",config.GetEndTimeHour(), config.GetEndTimeMinute());
    printf("#                                  #\n");
    printf("#  Tempo restante     : %02dh %02dmin  #\n",config.GetJobRemainingTimeHour(), config.GetJobRemainingTimeMinute());
    printf("#                                  #\n"  );
    printf("####################################\n\n");
    
    return(0);
}

#include <iostream>
#include <fstream> 
#include <vector> 
#include <sstream>
#include <ctime>

using namespace std;

class ConfigurationTime{
    public:

        unsigned int jobBeginHour;
        unsigned int jobBeginMinute;
        unsigned int lunchDurationHour;
        unsigned int lunchDurationMinute;
        unsigned int jobDurationHour;
        unsigned int jobDurationMinute;

        ConfigurationTime(){
            jobBeginHour = 0;
            jobBeginMinute = 0;
            lunchDurationHour = 0;
            lunchDurationMinute = 0;
            jobDurationHour = 0;
            jobDurationMinute = 0;

            SetLastConfigDay(0);
            SetLastConfigMonth(0);
            SetLastConfigYear(0);
        }

        ConfigurationTime(std::string filePath){
            _filePath = filePath;
            ReceiveConfigurationFromFile();
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

            jobBeginHour = getHour;
            jobBeginMinute = getMin;
        }

        void SetLunchDurationFromKeyboard(void){
            int getHour = 0, getMin = 0;
            string input = "";
            
            do{
                cout << "Digite duração do horário de almoço: ";
                getline(cin,input);

                for (char& c : input){
                    if (isalpha(c)){
                        c = ' ';
                    }
                }
                stringstream ss(input);     
                ss >> getHour >> getMin;

            }while ((getHour < 0 || getHour > 23) || (getMin < 0 || getMin > 59));

            lunchDurationHour = getHour;
            lunchDurationMinute = getMin;
        }

        void ReceiveConfigurationFromFile(void){
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

            jobBeginHour = stoi(line.at(0));
            jobBeginMinute  = stoi(line.at(1));

            lunchDurationHour = stoi(line.at(2));
            lunchDurationMinute  = stoi(line.at(3));
            jobDurationHour = stoi(line.at(4));
            jobDurationMinute = stoi(line.at(5));

            SetLastConfigDay(stoi(line.at(6)));
            SetLastConfigMonth(stoi(line.at(7)));
            SetLastConfigYear(stoi(line.at(8)));
            configFile.close();
        }

        void WriteFileAll(void){
            std::ofstream configFile;
            configFile.open(_filePath, std::ofstream::out | std::ofstream::trunc);
            configFile << "JOB_BEGIN_HOUR      = " << jobBeginHour << endl
                       << "JOB_BEGIN_MIN       = " << jobBeginMinute << endl
                       << "LUNCH_DURATION_HOUR = " << lunchDurationHour << endl
                       << "LUNCH_DURATION_MIN  = " << lunchDurationMinute << endl
                       << "JOB_DURATION_HOUR   = " << jobDurationHour << endl
                       << "JOB_DURATION_MIN    = " << jobDurationMinute << endl;
            WriteFileUpdateLastConfigData();
            configFile.close();
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
            std::tm tmFileTime {0};
            std::time_t timeNow = std::time(0);
            std::tm* tmTimeNow = localtime(&timeNow); 
            
            outputConfigFile  << "LAST_CONFIG_DAY     = " << tmTimeNow->tm_mday << endl
                              << "LAST_CONFIG_MON     = " << tmTimeNow->tm_mon + 1 << endl
                              << "LAST_CONFIG_YEAR    = " << tmTimeNow->tm_year - 100 << endl;
            outputConfigFile.close();
        }

        
        void SetLastConfigDay(unsigned int value){_lastConfig.day = value; }
        void SetLastConfigMonth(unsigned int value){_lastConfig.month = value; }
        void SetLastConfigYear(unsigned int value){_lastConfig.year = value; }
        void SetJobRemainingTimeHour(unsigned int value){_jobRemainingTime.hour = value; }
        void SetJobRemainingTimeMinute(unsigned int value){_jobRemainingTime.minute = value; }
        unsigned int GetLastConfigDay(void){ return _lastConfig.day; }
        unsigned int GetLastConfigMonth(void){ return _lastConfig.month; }
        unsigned int GetLastConfigYear(void){ return _lastConfig.year; }
        unsigned int GetJobRemainingTimeHour(void){ return _jobRemainingTime.hour; }
        unsigned int GetJobRemainingTimeMinute(void){ return _jobRemainingTime.minute; }

    private:
        string _filePath;
        enum TimeAndDateUnit {Hour, Minute, Day, Month, Year};
        struct LAST_CONFIG {unsigned int day ; unsigned int month; unsigned int year;} _lastConfig;
        struct JOB_REMAINING_TIME{unsigned int hour; unsigned int minute;} _jobRemainingTime;

        unsigned int CalculateJobRemainingTime(TimeAndDateUnit unit, struct JOB_REMAINING_TIME time){
            return 0;
        }



};

std::tm RemainingTime(std::tm* end, std::tm* begin);

int main(int argc, char* argv[])
{
    ConfigurationTime config = ConfigurationTime("Configuration");

    // Get current time
    std::tm tmFileTime {0};
    std::time_t timeNow = std::time(0);
    std::tm* tmTimeNow = localtime(&timeNow);

    // check if Configuration read from file is updated
    if (!config.FileConfigurationTimeIsUpdated()){
        config.SetJobBeginFromKeyboard();
        config.WriteFileAll();
    }

    std::tm tmEnd = {0};
    tmEnd.tm_hour = 2;//config.jobBeginHour + config.lunchDurationHour + config.jobDurationHour;
    tmEnd.tm_min =  70;// config.jobBeginMinute + config.lunchDurationMinute + config.jobDurationMinute;

    cout << tmEnd.tm_hour << "h" << tmEnd.tm_min << "min"<< endl;
    

    std::tm timeRemaining = RemainingTime(&tmEnd, tmTimeNow);

    return (0);

    char buffer[80];
    printf("\n####################################\n");
    printf("#                                  #\n"  );
    printf("#  Hora de entrada    : %02dh %02dmin  #\n",config.jobBeginHour, config.jobBeginMinute);
    printf("#  Duração do trabalho: %02dh %02dmin  #\n",config.jobDurationHour, config.jobDurationMinute);
    printf("#  Duração do almoço  : %02dh %02dmin  #\n",config.lunchDurationHour, config.lunchDurationMinute);
    printf("#                                  #\n"  );
    strftime (buffer,sizeof(buffer),"Hora de saida      : %Hh %Mmin",&tmEnd);
    printf("#  %s  #\n", buffer);
    printf("#                                  #\n");
    strftime (buffer,sizeof(buffer),"Tempo restante     : %Hh %Mmin",&timeRemaining);
    printf("#  %s  #\n", buffer);
    printf("#                                  #\n"  );
    printf("####################################\n\n");
    
}

std::tm RemainingTime(std::tm* end, std::tm* begin){
    std::tm mem;
    std::tm ret {0};
    
    mem = *end;
    *end = *begin;
    end->tm_hour = mem.tm_hour;
    end->tm_min = mem.tm_min;
    
    time_t difftimeSeconds = difftime(mktime(end), mktime(begin));

    mktime(end);

    cout << end->tm_hour << "h" << end->tm_min << "min"<< endl;

    ret.tm_hour = difftimeSeconds / 3600;
    ret.tm_min = (difftimeSeconds / 60) - (ret.tm_hour * 60);

    return ret;
}

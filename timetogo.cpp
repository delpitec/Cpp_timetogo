#include <iostream>
#include <fstream> 
#include <vector> 
#include <sstream>
#include <ctime>

using namespace std;

class ConfigurationTime{
    public:
        ConfigurationTime(){
            jobBeginHour = 0;
            jobBeginMinute = 0;
            lunchDurationHour = 0;
            lunchDurationMinute = 0;
            jobDurationHour = 0;
            jobDurationMinute = 0;

            lastDayConfig = 0;
            lastMonthConfig = 0;
            lastYearConfig = 0; 
        }

        ConfigurationTime(std::string filePath){
            _filePath = filePath;
            ReceiveConfigurationFromFile();
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

            lastDayConfig = stoi(line.at(6));
            lastMonthConfig = stoi(line.at(7));
            lastYearConfig = stoi(line.at(8));

            configFile.close();
        }
        void SendConfigurationToFile(tm* newCongigData){
            std::ofstream configFile;
            configFile.open(_filePath, std::ofstream::out | std::ofstream::trunc);
            configFile << "BEGIN_HOUR          = " << jobBeginHour << endl
                       << "BEGIN_MIN           = " << jobBeginMinute << endl
                       << "LUNCH_DURATION_HOUR = " << lunchDurationHour << endl
                       << "LUNCH_DURATION_MIN  = " << lunchDurationMinute << endl
                       << "JOB_DURATION_HOUR   = " << jobDurationHour << endl
                       << "JOB_DURATION_MIN    = " << jobDurationMinute << endl
                       << "LAST_DAY_CONFIG     = " << newCongigData->tm_mday << endl
                       << "LAST_MON_CONFIG     = " << newCongigData->tm_mon + 1 << endl
                       << "LAST_YEAR_CONFIG    = " << newCongigData->tm_year - 100 << endl;
            configFile.close();
        }
        
        unsigned int jobBeginHour;
        unsigned int jobBeginMinute;
        unsigned int lunchDurationHour;
        unsigned int lunchDurationMinute;
        unsigned int jobDurationHour;
        unsigned int jobDurationMinute;
        unsigned int lastDayConfig;
        unsigned int lastMonthConfig;
        unsigned int lastYearConfig;

    private:    
        string _filePath;
};

std::tm RemainingTime(std::tm* end, std::tm* begin);

int main(int argc, char* argv[])
{
    ConfigurationTime config = ConfigurationTime("/home/rafael.pino/time-to-go-app/Configuration");

    // Get current time
    std::tm tmFileTime {0};
    std::time_t timeNow = std::time(0);
    std::tm* tmTimeNow = localtime(&timeNow);

    // check if Configuration read from file is updated
    if (tmTimeNow->tm_mday != config.lastDayConfig ||
        tmTimeNow->tm_mon + 1  != config.lastMonthConfig || 
        tmTimeNow->tm_year - 100 != config.lastYearConfig){

        config.SetJobBeginFromKeyboard();
        config.SendConfigurationToFile(tmTimeNow);
    }

    std::tm tmEnd = {0};
    tmEnd.tm_hour = config.jobBeginHour + config.lunchDurationHour + config.jobDurationHour;
    tmEnd.tm_min = config.jobBeginMinute + config.lunchDurationMinute + config.jobDurationMinute;

    std::tm timeRemaining = RemainingTime(&tmEnd, tmTimeNow);
        
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

    ret.tm_hour = difftimeSeconds / 3600;
    ret.tm_min = (difftimeSeconds / 60) - (ret.tm_hour * 60);

    return ret;
}


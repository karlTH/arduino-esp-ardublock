/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/
#ifndef DUINOEDU_WEATHER_H
#define DUINOEDU_WEATHER_H

#include <Arduino.h>
#include <JsonListener.h>
#include <WundergroundAstronomy.h>
#include <WundergroundConditions.h>
#include <WundergroundForecast.h>

class Duinoedu_Weather
{
  public:
    Duinoedu_Weather(){;}
	int previousMillis_updateAstronomy  = 0;
	int previousMillis_updateConditions = 0;
	int previousMillis_updateForecast = 0;

	int count_updateAstronomy=0;
	int count_updateConditions=0;
	int count_updateForecast=0;

	String DUINOEDU_moonPctIlum ="";
	String DUINOEDU_moonAge ="";
	String DUINOEDU_moonPhase ="";
	String DUINOEDU_sunriseTime ="";
	String DUINOEDU_sunsetTime ="";
	String DUINOEDU_moonriseTime ="";
	String DUINOEDU_moonsetTime ="";

	String DUINOEDU_date ="";
	String DUINOEDU_windSpeed ="";
	String DUINOEDU_windDir ="";
	String DUINOEDU_currentTemp ="";
	String DUINOEDU_weatherIcon ="";
	//String DUINOEDU_weatherIconText ="";
	String DUINOEDU_weatherText ="";
	String DUINOEDU_humidity ="";
	String DUINOEDU_pressure ="";
	String DUINOEDU_dewPoint ="";
	String DUINOEDU_precipitationToday =""; 
	
     #define WEATHER_FORECAST_LENGTH 10 // A choisir en fonction du nombre de nesures
	String DUINOEDU_forecastIcon	[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastTitle	[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastLowTemp	[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastHighTemp[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastDay		[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastMonth	[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_forecastText	[WEATHER_FORECAST_LENGTH];
	String DUINOEDU_PoP				[WEATHER_FORECAST_LENGTH];

void updateAstronomy ( String _apiKey, String _language, String _country, String _city, int _time){
WundergroundAstronomy wunderground(false);

 if ( ((millis() - previousMillis_updateAstronomy) > (_time * 60000)) || ( count_updateAstronomy == 0)) {
		count_updateAstronomy=1;
		previousMillis_updateAstronomy=millis();
		WGAstronomy astronomy;
		wunderground.updateAstronomy(&astronomy, _apiKey, _language, _country, _city);

		DUINOEDU_moonPctIlum		=	astronomy.moonsetTime;
		DUINOEDU_moonAge			=	astronomy.moonAge;
		DUINOEDU_moonPhase			=	astronomy.moonPhase;
		DUINOEDU_sunriseTime		=	astronomy.sunriseTime;
		DUINOEDU_sunsetTime			=	astronomy.sunsetTime;
		DUINOEDU_moonriseTime		=	astronomy.moonriseTime;
		DUINOEDU_moonsetTime		=	astronomy.moonsetTime;
    }	
}

void updateConditions ( String _apiKey, String _language, String _country, String _city, int _time){
WundergroundConditions wunderground(true);

 if ( ((millis() - previousMillis_updateConditions) > (_time * 60000)) || ( count_updateConditions == 0)) {
		count_updateConditions=1;
		previousMillis_updateConditions=millis();
		WGConditions conditions;
		wunderground.updateConditions(&conditions, _apiKey, _language, _country,_city);

		DUINOEDU_date				=	conditions.date;
		DUINOEDU_windSpeed			=	conditions.windSpeed;
		DUINOEDU_windDir			=	conditions.windDir;
		DUINOEDU_currentTemp		=	conditions.currentTemp;
		DUINOEDU_weatherIcon		=	conditions.weatherIcon;
//		DUINOEDU_weatherIconText	=	conditions.weatherIconText;
		DUINOEDU_weatherText		=	conditions.weatherText;
		DUINOEDU_humidity			=	conditions.humidity;
		DUINOEDU_pressure			=	conditions.pressure;
		DUINOEDU_dewPoint			=	conditions.dewPoint;
		DUINOEDU_precipitationToday	=	conditions.precipitationToday;
    }	
}

void updateForecast ( String _apiKey, String _language, String _country, String _city, int _time, int _maxForecasts){
WundergroundForecast wunderground(true);

 if ( ((millis() - previousMillis_updateForecast) > (_time * 60000)) || ( count_updateForecast == 0) ) {
		count_updateForecast=1;
		previousMillis_updateForecast=millis();
		WGConditions conditions;
		WGForecast forecasts[_maxForecasts];
		wunderground.updateForecast(forecasts,_maxForecasts, _apiKey, _language, _country,_city);
		
		//--Initialisation (1 fois:static)
		// static bool initRequired = true;
		// if(initRequired){
		  // for(uint8_t i=0 ; i<WEATHER_FORECAST_LENGTH; i++){
			// DUINOEDU_forecastIcon[i]		=	"";
			// DUINOEDU_forecastTitle[i]		=	"";
			// DUINOEDU_forecastLowTemp[i]     =	"";
			// DUINOEDU_forecastHighTemp[i]	=	"";
			// DUINOEDU_forecastDay[i]			=	"";
			// DUINOEDU_forecastMonth[i]		=	"";
			// DUINOEDU_forecastText[i]		=	"";
			// DUINOEDU_PoP[i]					=	"";
			// initRequired = false;			
		  // }
		// }
		
		//--Mesures
		for (uint8_t i=0; i<_maxForecasts ; i++){
			DUINOEDU_forecastIcon[i]		=	forecasts[i].forecastIcon;
			DUINOEDU_forecastTitle[i]		=	forecasts[i].forecastTitle;
			DUINOEDU_forecastLowTemp[i]		=	forecasts[i].forecastLowTemp;
			DUINOEDU_forecastHighTemp[i]	=	forecasts[i].forecastHighTemp;
			DUINOEDU_forecastDay[i]			=	forecasts[i].forecastDay;
			DUINOEDU_forecastMonth[i]		=	forecasts[i].forecastMonth;
			DUINOEDU_forecastText[i]		=	forecasts[i].forecastText;
			DUINOEDU_PoP[i]					=	forecasts[i].PoP;
		}
		
    }	
}


};

#endif
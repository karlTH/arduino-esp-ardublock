/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/
#ifndef DUINOEDU_IRREMOTEESP8266_H
#define DUINOEDU_IRREMOTEESP8266_H

#include <IRremoteESP8266.h>
#include <IRsend.h>  // Needed if you want to send IR commands.
#include <IRrecv.h>  // Needed if you want to receive IR commands.




class IRrecv2 : public IRrecv
{
  public:
#define SERIALPLUS_MEMORISER 				-1
#define IRREMOTE_MEMORISER_TOUCHE			-1
#define IRREMOTE_MEMORISER      			-1
#define DUINOEDU_MEMORISER_TOUCHE			-1
#define DUINOEDU_MEMORISER					-1

#define SERIALPLUS_MEMORISER_200_MS 		200
#define IRREMOTE_MEMORISER_200_MS 		    200
#define DUINOEDU_MEMORISER_200_MS 		    200

#define DUINOEDU_MEMORISER_500_MS 			500
#define DUINOEDU_MEMORISER_1000_MS 			1000

#define SERIALPLUS_MEMORISER_2000_MS 		2000
#define IRREMOTE_MEMORISER_2000_MS 		    2000
#define DUINOEDU_MEMORISER_2000_MS 			2000

#define DUINOEDU_MEMORISER_5000_MS 			5000
#define DUINOEDU_MEMORISER_10000_MS 		10000

#define SERIALPLUS_NE_PAS_MEMORISER 		0
#define IRREMOTE_NE_PAS_MEMORISER 		    0
#define DUINOEDU_NE_PAS_MEMORISER 			0


 /*EDU US*/ decode_results results;
  /*EDU US*/ unsigned long m_lastCode;
  /*EDU US*/ unsigned long m_dateCode;          // Date à laquelle le dernier code à été lu
  /*EDU US*/ unsigned long m_lifeCode;			// Durée de vie du code
  /*EDU US*/ bool m_longPush;					// True si appui long en cours
  /*EDU US*/ unsigned long m_dateLongPush;		// Date à laquelle la dernier appui long a été demandé
  /*EDU US*/ unsigned long m_lastDateLongPush;  // Date à laquelle l'avant dernier appui long à été enregistré
  /*EDU US*/ unsigned long m_lifeLongPush;		// Durée de vie minimum d'un appui long (évite les rebondissements)
volatile irparams_t irparams;

// IRrecv2  (uint16_t recvpin) const {
	// IRrecv(recvpin);
	// irparams.recvpin = recvpin;
	// }
	
IRrecv2  (uint16_t recvpin)  : IRrecv(recvpin){}
//IRrecv2 const IRrecv&(uint16_t recvpin);
 
 
// void enableIRIn() {
  // _irrecv.enableIRIn();
  // irparams.rcvstate = STATE_IDLE;
  // irparams.rawlen = 0;
  // irparams.overflow = false;
// } 
  
  
unsigned long codeIrReadLong(int option){
  // 3 options de décodage :
  // Option = -1 : (défaut) Le code doit être lu en "live"    
  // Option = 0  : (IRREMOTE_DERNIER_CODE) Le code courant est le dernier code lu
  // Option = xx : Le code à une durée de vie de xx milisecondes
  
  // Sauvegarde durée de vie du code
    /**/if(option>0){m_lifeCode=option;}
    /**/else{m_lifeCode=0;}
    
    unsigned long codeTouche=0;
    //--- Si réussite d'un décodage...
    if ( decode( &results ) ){ 
      //-- On lit le code d'une manière inconditionnelle
        codeTouche = results.value ; 
      //-- On mémorise ce code
      //-- Si ce n'est pas le code d'appui long
        if(codeTouche!=4294967295 && codeTouche!=0){
          m_longPush=false;
          //-On mémorise comme code représentatif de la touche appuyée
            m_lastCode=codeTouche;
            Serial.print("COURT:"); Serial.println(codeTouche);
          //-On déclare immédiatement que ce n'est pas ou plus un appui long  
            m_longPush=false;
          //-On enregistre la date
          m_dateCode=millis();
        }
      //-- Si c'est le code d'appui long (le même pour toutes les touches)  
        else if(codeTouche==4294967295){
          //-On déclare que c'est un appuis long
            Serial.print("LONG:");Serial.println(codeTouche);
            m_longPush=true;
            //--- On sauvegarde la date de l'avant dernier code d'appui long
            m_lastDateLongPush=m_dateLongPush;
            m_dateLongPush=millis();
        }
          
        //- On enregistre la date
          //m_dateCode=millis();
      resume();
    }
  //-------- Gestion des appuis longs
    if(m_longPush){
      if(millis()<m_lastDateLongPush+400){
        //-- On est en mode appuis long => les options ne comptent pas
        //Serial.print("?");Serial.println(millis());
          return m_lastCode;
      }
      else{
        //-- Appui long périmé
          //Serial.print("!");Serial.println(millis());
          m_longPush=false;
          return 0;
          
      }
    }
        

    else{
      //-- On est en mode appui court
      //m_longPush=false;
    //----- Gestion : Temps réel (pas d'effet mémoire)  
      if(option==0){return codeTouche;} 
    //-----  Gestion : Tempo
      if(option>0){
        //-- Retourne ancien code
          if(millis()<m_dateCode+m_lifeCode){
            Serial.print("datCo");Serial.println(m_dateCode);
            Serial.print("anc");Serial.println(millis());
            return m_lastCode;
            
          }
        //-- Retourne code actuel
          else{
            return codeTouche;
            
          }
      }
    //----- Gestion : Rappel de la dernière touche
      if(option==-1){return m_lastCode;}      
    
    }
    
}


/*EDU US*/ bool testerTouche(String touch, int option){
	unsigned long code=codeIrReadLong(option);
	/*if(code==0){
		return false;
	}
	*/
	//Appui court 16... / Appui long 42...
	Serial.println(code);
	if((touch=="AUCUNE" ||  touch=="NOTHING")   					        && (code==0 ))				{return true;}
	else if((touch=="POWER"||touch=="ON/OFF"||touch=="ON")  			    && (code==16753245 ))		{return true;}
	else if(touch=="MENU" 													&& (code==16769565 ))		{return true;}
	else if(touch=="TEST" 													&& (code==16720605 ))		{return true;}
	else if((touch=="+" || touch=="PLUS")									&& (code==16712445 )) 		{return true;}
	else if(touch=="ANNULER" 												&& (code==16761405 ))		{return true;}
	else if((touch=="RETOUR" || touch=="GAUCHE") 							&& (code==16769055 ))		{return true;}
	else if(touch=="LECTURE" 												&& (code==16754775 ))		{return true;}
	else if((touch=="AVANCE" || touch=="DROITE")   							&& (code==16748655 ))		{return true;}
	else if(touch=="0"    													&& (code==16738455 ))		{return true;}
	else if((touch=="-" ||  touch=="MOINS") 	 							&& (code==16750695 ))		{return true;}
	else if(touch=="C"  													&& (code==16756815 ))		{return true;}
	else if(touch=="1"    													&& (code==16724175 ))		{return true;}
	else if(touch=="2"    													&& (code==16718055 ))		{return true;}
	else if(touch=="3"    													&& (code==16743045 ))		{return true;}
	else if(touch=="4"    													&& (code==16716015 ))		{return true;}
	else if(touch=="5"    													&& (code==16726215 ))		{return true;}
	else if(touch=="6"    													&& (code==16734885 ))		{return true;}
	else if(touch=="7"    													&& (code==16728765 ))		{return true;}
	else if(touch=="8"    													&& (code==16730805 ))		{return true;}
	else if(touch=="9"    													&& (code==16732845 ))		{return true;}
	

	return false;
		
}
};
// class IRsend2
// {
	 // public:
// #define SERIALPLUS_MEMORISER 				-1
// #define IRREMOTE_MEMORISER_TOUCHE			-1
// #define IRREMOTE_MEMORISER      			-1
// #define DUINOEDU_MEMORISER_TOUCHE			-1
// #define DUINOEDU_MEMORISER					-1

// #define SERIALPLUS_MEMORISER_200_MS 		200
// #define IRREMOTE_MEMORISER_200_MS 		    200
// #define DUINOEDU_MEMORISER_200_MS 		    200

// #define DUINOEDU_MEMORISER_500_MS 			500
// #define DUINOEDU_MEMORISER_1000_MS 			1000

// #define SERIALPLUS_MEMORISER_2000_MS 		2000
// #define IRREMOTE_MEMORISER_2000_MS 		    2000
// #define DUINOEDU_MEMORISER_2000_MS 			2000

// #define DUINOEDU_MEMORISER_5000_MS 			5000
// #define DUINOEDU_MEMORISER_10000_MS 		10000

// #define SERIALPLUS_NE_PAS_MEMORISER 		0
// #define IRREMOTE_NE_PAS_MEMORISER 		    0
// #define DUINOEDU_NE_PAS_MEMORISER 			0

// void sendNumber(int32_t _number, uint16_t _interval){
	
	// ----Gestion des intervals d'envoi
		// static uint32_t lasteTime = 0;
		// if(millis() < lasteTime + _interval){
			// --- Trop tôt pour envoyer à nouveau
			// return;
		// }else{
			// --- Il faut envoyer une donnée
			// IRsend irsend(0);
			// irsend.sendNEC(_number, 20);
			// lasteTime = millis();
		// }
	  
  // }

// };

#endif
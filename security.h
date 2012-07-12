
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

#ifndef securityH
#define securityH

/** \file security.h
    \brief Contains definition of a class TSecurity.

    TSecurity is a main class of the FINAL's fixed income part. Basically
    all classes representing fixed income securities (plus floating rate
    notes) have TSecurity as a base classes and thus inherit its
    methods.

    Related topics:
    - TBond
    - TZeroCpn
    - TFRN

    Examples:
    - bond_example.cpp
    - etc...
*/

#include "defs.h"
#include "calendar.h"
#include <map>

#include "../utils/utils.h"
using namespace utils;

namespace final {

#ifdef Yield
#undef Yield
#endif

typedef std::map<TDate, floating > TCashflows;

typedef enum {
  SEC_BOND = 0,
  SEC_BILL = 1,
  SEC_FRN = 2,
  SEC_CD = 3,
  SEC_BOND_ANNUAL = 10,
  SEC_BTPS = 11,
  SEC_SPGB = 12,
  SEC_FRTR = 13,
  SEC_BTNS = 14,
  SEC_UKT = 15,
  SEC_UST = 16,
  SEC_CZGB = 17,
  SEC_CCTS = 18,
  SEC_JGB = 19,
  SEC_ACGB = 20,
  SEC_ACTB = 21,
  SEC_JGTB = 50,
  SEC_USB = 51
} TSecurityModel;

class TSecurity
{
protected:

  TDate      maturity;        // maturita
  TDate      issued;          // den vydani
  int        basis;           // day-count konvence pro nabehly urok
  const TCalendar* calendar;        // kalendar pro TrueYield funkce
                              // (pokud neni nastaven - 0 - vyuziva se standardni
                              // "prazdny" kalendar, ktery preskakuje vikendy)
  int        mmktconv;        // money-marketova konvence, ktera se standardne
                              // pouzije pri vypoctu money-market yieldu

  virtual void     Readjust() {};
                   // interni funkce prenastavujici interni promene

public:
                   TSecurity( const TDate& amaturity, const TDate& aissued, int abasis );
                   // konstruktor
  virtual          ~TSecurity();

  virtual TString  SecurityDescriptor() const;

  virtual int      IsFloater() const { return false; }
  virtual int      IsBill() const    { return false; }
  virtual int      IsBond() const    { return false; }
  virtual int      IsCD() const      { return false; }
  virtual int      HasCoupon() const { return false; }

  virtual char*    SecurityType() const = 0;
  virtual int      SecurityTypeNumber() const = 0;

  virtual floating MarketValue( floating anominal, const TDate& asettle, floating apx ) const;
  virtual floating MarketValueY( floating anominal, const TDate& asettle, floating aytm ) const;

  virtual floating BpvAmount( floating anominal, const TDate& asettle, floating apx ) const;
  virtual floating BpvAmountY( floating anominal, const TDate& asettle, floating aytm ) const;

                   // vetsina financnich funkci existuje ve dvou verzich
                   // napr. Duration / DurationY
                   // funkce zakoncene 'Y' nemaji jako vstupni parametr cenu
                   // ale conventional yield do maturity
  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;
  virtual floating Duration( const TDate& asettle, floating apx ) const
                   { return DurationY( asettle, ConvYield(asettle,apx) ); }
  virtual floating MDuration( const TDate& asettle, floating apx, int adjust=false ) const
                   { return MDurationY( asettle, ConvYield(asettle,apx) ); }
  virtual floating Convexity( const TDate& asettle, floating apx ) const
                   { return ConvexityY( asettle, ConvYield(asettle,apx) ); }
  virtual floating ConvexityY( const TDate& asettle, floating aytm ) const;
  virtual floating Risk( const TDate& asettle, floating apx ) const
                   { return RiskY( asettle, ConvYield(asettle,apx) ); }
  virtual floating RiskY( const TDate& asettle, floating aytm ) const ;


          TDate    StandardSettlement() const;
                   // StandardSettlement vraci datum standardniho settlementu
                   // vzhledem k systemovemu casu
  virtual TDate    StandardSettlement( const TDate& atradedate ) const = 0;
                   // vraci standardni datum settlementu vzhledem k datumu
                   // obchodu atradedate

  virtual TDate    WorkDay( const TDate& adate, int n ) const;
                   // vraci datum o n pracovnich dnu dal
                   // bere v potaz kalendar, pokud je nastaven, jinak pouze
                   // preskakuje vikendy


                   // ConvYield je funkce navracejici k danemu dni a cene
                   // odpovidajici 'conventional yield'. typ konvencniho yieldu
                   // se muze u trid odvozenych od TSecurity lisit
                   // (viz. napr. TBTNS)
  virtual floating ConvYield( const TDate& asettle, floating apx,
                              int around=true ) const
                   { return Yield( asettle, apx ); }
                   // parova funkce k ConvYield provadejici druhou smer vypoctu
  virtual floating ConvPrice( const TDate& asettle, floating aytm,
                              int around=true ) const
                   { return Price( asettle, aytm ); }

  virtual floating CompYield( const TDate& asettle, floating apx, int acomp ) const
                   { return ConvYield( asettle, apx ); }
                   // navraci ConvYield v compoundingu acomp

                   // Yield a Price jsou parove funkce pro vypocet
                   // standardniho vynosu do splatnosti
  virtual floating Yield( const TDate& asettle, floating apx ) const = 0;
  virtual floating Price( const TDate& asettle, floating aytm ) const = 0;

                   // TrueYield a TruePrice jsou funkce, ktere oproti Yield/Price
                   // berou ohled na kalendar a v pripade svatku/vikendu
                   // jsou penezni toky posunuty na prvni pracovni den
  virtual floating TrueYield( const TDate& asettle, floating apx ) const = 0;
  virtual floating TruePrice( const TDate& asettle, floating aytm ) const = 0;

                   // MMktYield a MMktPrice jsou funkce vracejici
                   // jednoduchy vynos (cenu vypocitanou z jednoducheho vynosu).
                   // Pokud je bumpholidays nastaveno na true,
                   // V potaz je bran kalendar, pokud tedy maturita nastane o
                   // o vikendu (ev. svatku), penezni tok je posunut na prvni
                   // pracovni den. V pripade vynechani parametru abasis
                   // se jako konvence pouzije mmktconv.
  virtual floating MMktYield( const TDate& asettle, floating apx,
                              int bumpholidays=true, int abasis=-1 ) const;
  virtual floating MMktPrice( const TDate& asettle, floating ayield,
                              int bumpholidays=true, int abasis=-1 ) const;

                   // Discount/DiscountToPrice
                   // jsou dalsi z money-marketovych funkci, namisto
                   // vynosu vypocitavaji diskont.
  virtual floating Discount( const TDate& asettle, floating apx, int abasis=-1 ) const;
  virtual floating DiscountToPrice( const TDate& asettle, floating adisc, int abasis=-1 ) const;


  virtual floating AccruedInterest( const TDate& asettle ) const = 0;
                   // nabehly urok ke dni asettle

  virtual TDate    PrevCouponDate( const TDate& asettle ) const = 0;
  virtual TDate    NextCouponDate( const TDate& asettle ) const = 0;
  virtual floating NextCouponSize( const TDate& asettle ) const;
  virtual floating Redemption() const { return 100.0; }

  virtual TDate    RealSettlementDate( const TDate& asettle ) const;
  virtual int      NextCashflow( const TDate& asettle, TDate& anextone,
                                 floating& aamount ) const = 0;
                   // vraci pocet peneznich toku od asettle do maturity
                   // pokud vrati nulu (=zadne toky), anextone a aamount jsou
                   // nezmeneny, jinak obsahuji objem a datum pristiho toku
  virtual floating GetNextCashflow( const TDate& asettle ) const;
  virtual int      RealNextCashflow( const TDate& asettle, TDate& anextone,
                                     floating& aamount) const;
  virtual int      CashflowsTillMaturity( const TDate& asettle ) const = 0;
                   // vraci pocet peneznich toku od asettle do maturity
  virtual int      RealCashflowsTillMaturity( const TDate& asettle ) const;
  virtual floating CashflowAmount( const TDate& asettle ) const = 0;
                   // vraci objem penezniho toku v den asettle
  virtual floating RealCashflowAmount( const TDate& asettle ) const;

  virtual floating SumCashflows( const TDate& afrom, const TDate& ato,
                                 floating areinvestrate, int abasis );
                   // secte FV ke dni ato vsech toku od afrom do ato (vcetne)
                   // (pokud je ato==afrom, vraci 0)

  virtual void     GetCashflows( const TDate& asettle, TCashflows &cfs,
                                 floating aamount = 100.0 ) const;

                   // nasledujici funkce vraceji parametry daneho cenneho papiru
          TDate    Maturity() const   { return maturity; }
          TDate    Issued() const     { return issued; }
          int      Basis() const      { return basis; }
          int      MMktConv() const   { return mmktconv; }

          const TCalendar* Calendar() const;

          void     Set( const TDate& amaturity, const TDate& aissued, int abasis );
                   // umoznuje prenastavit nektere parametry
          void     SetCalendar( const TCalendar* acalendar );
                   // nastavi novy kalendar
          void     SetMMktConv( int ammktconv ) { mmktconv = ammktconv; }
};

} // end of namespace final

#endif

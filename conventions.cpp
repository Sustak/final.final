// Financial Analytics Library (FINAL)
// Copyright (c) 2004 - 2012 by Marek Sestak, marek.sestak@gmail.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "conventions.h"

#include "exceptions.h"

namespace final {

floating YearFrac( const TDate& astart, const TDate& aend, int abasis )
{

  int ds,ms,ys;                        // day, month, year - start date
  int de,me,ye;                        // day, month, year - end date
  int ss,se;                           // seriove cisla zacatku(ss) a konce(se)
  TDate start,end,dummy_dt;

  int num;                             // 'num'erator
  floating denum;                      // 'denum'erator
  int sign;

  start = astart;
  end   = aend;

  start.Validate();
  end.Validate();

  if( start.Serial()<=end.Serial() )   // pokud je obdobi zadano obracene
  {                                    // nejprve datumy prohodime
    sign=1;                            // a pracujeme s nimi, jako kdyby
  }                                    // byly zadany normalne a nakonci
  else                                 // to cele vynasobime sign
  {
    sign=-1;
    dummy_dt=start;
    start=end;
    end=dummy_dt;
  }

  start.Get( ds,ms,ys );
  end.Get( de,me,ye );
  
  ss=start.Serial();                  // pri vypoctu poctu dnu
  se=end.Serial();                    // se s tim ruzne hejba, proto si to
                                       // radsi zapamatujeme hned
  
  // ACT_ACT_ISDA je svou povahou trochu odlisna od ostatnich konvenci
  // spocteme ji zvlast a rovnou vratime vysledek
  if( abasis==ACT_ACT_ISDA || abasis==ACT_ACT_ISDA_NONEOM ) {

      if( IsLeap(ys) ) {
          denum = 366.0;
      }
      else {
          denum = 365.0;
      }
      if( ys==ye ) {
          return (se-ss)/denum;
      }
      
      floating denumEnd;
      if( IsLeap(ye) ) {
          denumEnd = 366.0;
      }
      else {
          denumEnd = 365.0;
      }
      int yearStartSerial = TDate(1,1,ye).Serial();
      return (se-yearStartSerial)/denumEnd + (yearStartSerial-ss)/denum;
  }

  num = 0;
  denum = 0;

  if( abasis>=NONEOM_start )
    abasis = abasis-NONEOM_start;

  switch( abasis )                     // ve funknci YearFrac neni treba NOEOM
                                       // modifikator brat v uvahu
  {
    case US_30_360:
    case US_30_365:
    case US_30_ACT:
      if( de==31 )                     // americka konvence 30/360 (365,ACT)
      {                                // ma spoustu zajimavosti:
        de=30; num=1;                  // - 1.5.-31.5. = 30
      }                                // - prestupne roky 29.2.-1.3. = 1
      if( start.IsMonthEnd() || ds==30 )
      {                                // - neprestupne    28.2.-1.3. = 1
        ds=1; ms++;                    //
        if( ms==13 )                   // princip uprav
                                       // 1.pokud konec obdobi padne na 31. den
        {                              //   posuneme to na 30. a pricteme 1
          ms=1;                        // 2.pokud zacatek obdobi padne na
          ys++;                        //   konec mesice, posuneme to na zacatek
        }                              //   dalsiho mesice a pricteme 1
        num=1;                         // 3.pokud nastane zaroven 1&2, nepricteme
      }                                //   2, ale jen 1

      num += (ye-ys)*360 + (me-ms)*30 + (de-ds);
      break;
    case ISMA_30_360:
    case ISMA_30_365:
    case ISMA_30_ACT:                  // evropska ISMA 30/360 (365,ACT)
      if( ds==31 )                     // je v porovnani s americkou jednodussi
        ds = 30;                       // staci posunout 31.dny na 30.
      if( de==31 )
        de = 30;
      num = (ye-ys)*360 + (me-ms)*30 + (de-ds);
      break;
    case NL_360:
    case NL_365:
    case NL_ACT:
      num = (ye-ys)*365;
      if( me==2 && de==29 )
        de = 28;
      if( ms==2 && ds==29 )
        ds = 28;
      dummy_dt.Set(ds,ms,1997);
      num -= dummy_dt.Serial();
      dummy_dt.Set(de,me,1997);
      num += dummy_dt.Serial();
      break;
    case ACT_ACT:
    case ACT_360:                      // u ACT je to nejjednodussi, staci
    case ACT_365:                      // odecist seriovy cisla
      num = end.Serial() - start.Serial();
      break;
  }
  switch( abasis )
  {
    case US_30_360:
    case ISMA_30_360:
    case ACT_360:
    case NL_360:
      denum=360;
      break;
    case US_30_365:
    case ISMA_30_365:
    case ACT_365:
    case NL_365:
      denum=365;
      break;
    case US_30_ACT:
    case ISMA_30_ACT:
    case ACT_ACT:
    case NL_ACT:
      TDate dt1,dt2;
      int ndays;
      int yearend;

      if( start.IsLeap() )            // zacina obdobi v prestupnem roce?
      {
        dt1=TDate(29,2,ys);            // obsahuje obdobi den 29.2.?
        if( dt1.Serial()>=ss && dt1.Serial()<=se )
          ndays=366;                   // ano...proto je nutne vzit 366 dnu
        else
          ndays=365;                   // je to sice prestupny rok, ale
                                       // 29.2.byl uz pred zacatkem obdobi
      }
      else
        ndays=365;                     // neprestupny rok, proto 365

      dt2=TDate(31,12,ys);
      yearend=dt2.Serial();
      if( se<=yearend )
        denum=ndays;                   // cele obdobi lezi v jednom kalendarnim
      else                             // roce
      {
        denum=(floating)(yearend-ss)/ndays;

        if( end.IsLeap() )             // konci obdobi v prestupnem roce?
        {
          dt1=TDate(29,2,ye);          // obsahuje obdobi den 29.2.?
          if(dt1.Serial()<=se)
            ndays=366;
          else
            ndays=365;
        }
        else
          ndays=365;

        dt2=TDate(31,12,ye-1);
        denum+=(floating)(se-dt2.Serial())/ndays + (ye-ys-1);
        denum=(floating)(se-ss)/(denum);
      }


      break;
  }

  floating result = sign*num/denum;
  return result;
}

int NDays( const TDate& afrom, const TDate& ato, int abasis )
{
  int ds,ms,ys;                        // day, month, year - start date
  int de,me,ye;                        // day, month, year - end date

  int num=0;                           // 'num'erator
  int yearStartSerial;

  afrom.Validate();
  ato.Validate();

  afrom.Get( ds,ms,ys );
  ato.Get( de,me,ye );

  switch( abasis<NONEOM_start? abasis:abasis-NONEOM_start )
                                       // ve funknci YearFrac neni treba NOEOM
                                       // modifikator brat v uvahu
  {
    case US_30_360:
    case US_30_365:
    case US_30_ACT:
      if( de==31 )                     // americka konvence 30/360 (365,ACT)
      {                                // ma spoustu zajimavosti:
        de=30; num=1;                  // - 1.5.-31.5. = 30
      }                                // - prestupne roky 29.2.-1.3. = 1
      if( afrom.IsMonthEnd() || ds==30 )
      {                                // - neprestupne    28.2.-1.3. = 1
        ds=1; ms++;                    //
        if( ms==13 )                   // princip uprav
                                       // 1.pokud konec obdobi padne na 31. den
        {                              //   posuneme to na 30. a pricteme 1
          ms=1;                        // 2.pokud zacatek obdobi padne na
          ys++;                        //   konec mesice, posuneme to na zacatek
        }                              //   dalsiho mesice a pricteme 1
        num=1;                         // 3.pokud nastane zaroven 1&2, nepricteme
      }                                //   2, ale jen 1

      num += (ye-ys)*360 + (me-ms)*30 + (de-ds);
      break;
    case ISMA_30_360:
    case ISMA_30_365:
    case ISMA_30_ACT:                  // evropska ISMA 30/360 (365,ACT)
      if( ds==31 )                     // je v porovnani s americkou jednodussi
        ds = 30;                       // staci posunout 31.dny na 30.
      if( de==31 )
        de = 30;
      num = (ye-ys)*360 + (me-ms)*30 + (de-ds);
      break;
    case ACT_ACT:
    case ACT_360:                      // u ACT je to nejjednodussi, staci
    case ACT_365:                      // odecist seriovy cisla
      num = ato.Serial() - afrom.Serial();
      break;
    case ACT_ACT_ISDA:      
        floating denumNext, denumPrev;
        if( ato.IsLeap() ) {
            denumNext = 366.0;
        }
        else {
            denumNext = 365.0;
        }
        if( afrom.IsLeap() ) {
            denumPrev = 366.0;
        }
        else {
            denumPrev = 365.0;
        }
        yearStartSerial = TDate(1,1,ye).Serial();
        num = ( (ato.Serial()-yearStartSerial)*denumPrev
                + (yearStartSerial-afrom.Serial())*denumNext ) / 365.0;
        break;
      
    case NL_360:
    case NL_365:
    case NL_ACT:
      TDate dummy_dt;
      num = (ye-ys)*365;
      if( me==2 && de==29 )
        de = 28;
      if( ms==2 && ds==29 )
        ds = 28;
      dummy_dt.Set(ds,ms,1997);
      num -= dummy_dt.Serial();
      dummy_dt.Set(de,me,1997);
      num += dummy_dt.Serial();
      break;
  }

  return num;
}

floating Denum( const TDate& prevcpn, const TDate& nextcpn,
                int afrequency, int abasis )
{
  floating denum;
  switch( abasis<NONEOM_start? abasis : abasis-NONEOM_start )
  {
    case ACT_ACT:
    case ISMA_30_ACT:
    case US_30_ACT:
    case NL_ACT:
      denum = nextcpn.Serial()-prevcpn.Serial();
      break;
    case ACT_ACT_ISDA:
        floating denumNext, denumPrev;
        if( nextcpn.IsLeap() ) {
            denumNext = 366.0;
        }
        else {
            denumNext = 365.0;
        }
        if( prevcpn.IsLeap() ) {
            denumPrev = 366.0;
        }
        else {
            denumPrev = 365.0;
        }
        denum = denumNext * denumPrev / 365.0 / afrequency;
        break;
    case ACT_360:
    case ISMA_30_360:
    case US_30_360:
    case NL_360:
      denum = (floating)360.0/afrequency;
      break;
    case ACT_365:
    case ISMA_30_365:
    case US_30_365:
    case NL_365:
      denum = (floating)365.0/afrequency;
      break;
    default:
      throw TExFINAL( "Unknown daycount basis. Accrued interest cannot be expressed." );
  }
  return denum;
}

} // end of namespace

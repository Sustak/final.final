/*
 * @license
 */
package finale;

import finale.utils.Date;

/**
 *
 * @author Marek Šesták
 */
public class Utils {

	/**
	 * Returns the fraction of the year representing the number of days between
	 * start and end dates according to the specified daycount convention.
	 * @param start beginning of the period
	 * @param end end of the period
	 * @param convention daycount convention (see {@link DaycountConvention})
	 * @return The whole number of days between the start date and the end date
	 * is first calculated using the specified daycount counvention and
	 * then divided by number of days that represent the year in the same convention.
	 * <P> Please note, that a negative number is returned if the start date
	 * preceeds the end date (that is result is <code>YearFrac(end,start,convention)*(-1.0)</code>).
	 * @see DaycountConvention
	 */
	public static double YearFrac(Date start, Date end, DaycountConvention convention) {

		return NDays(start, end, convention) / Denum(start, end, convention);

	}

	public static int NDays(Date start, Date end, DaycountConvention convention) {

		// ss and se contain serial numbers corresponding with start and end,
		// if start and end dates are in a reverse order, we will swap them,
		// make a note in the sign variable (-1), and the returned value will
		// then be negative.
		//
		// dStart and dEnd contain beginning and end of the period
		// irrespective of in what order they were supplied by the caller.
		int sign;
		Date dStart, dEnd;
		if (start.getSerial() > end.getSerial()) {
			sign = -1;
			dStart = end;
			dEnd = start;
		} else {
			sign = 1;
			dStart = start;
			dEnd = end;
		}

		// ds, ms, ys - day of month, month, year of the start date
		// de, me, ye - day of month, month, year of the end date
		int ds, ms, ys;
		int de, me, ye;

		ds = dStart.getDay();
		ms = dStart.getMonth();
		ys = dStart.getYear();
		de = dEnd.getDay();
		me = dEnd.getMonth();
		ye = dEnd.getYear();

		// num and denum are two variables that represent the
		// 'num'erator and 'denum'erator of the ratio that expresses the
		// year fraction
		int num = 0;

		// NON_EOM modifier doesn't need to be taken into account,
		// we'll modify the convention...
		switch (DaycountConvention.toDaycountConventionEOM(convention)) {
			case US_30_360:
			case US_30_365:
			case US_30_ACT:
				if (de == 31) // americka konvence 30/360 (365,ACT)
				{                                // ma spoustu zajimavosti:
					de = 30;
					num = 1;                  // - 1.5.-31.5. = 30
				}                                // - prestupne roky 29.2.-1.3. = 1
				if (dStart.isMonthEnd() || ds == 30) {                                // - neprestupne    28.2.-1.3. = 1
					ds = 1;
					ms++;                    //
					if (ms == 13) // princip uprav
					// 1.pokud konec obdobi padne na 31. den
					{                              //   posuneme to na 30. a pricteme 1
						ms = 1;                        // 2.pokud zacatek obdobi padne na
						ys++;                        //   konec mesice, posuneme to na zacatek
					}                              //   dalsiho mesice a pricteme 1
					num = 1;                         // 3.pokud nastane zaroven 1&2, nepricteme
				}                                //   2, ale jen 1

				num += (ye - ys) * 360 + (me - ms) * 30 + (de - ds);
				break;
			case ISMA_30_360:
			case ISMA_30_365:
			case ISMA_30_ACT:                  // evropska ISMA 30/360 (365,ACT)
				if (ds == 31) // je v porovnani s americkou jednodussi
				{
					ds = 30;
				}                       // staci posunout 31.dny na 30.
				if (de == 31) {
					de = 30;
				}
				num = (ye - ys) * 360 + (me - ms) * 30 + (de - ds);
				break;
			case NL_360:
			case NL_365:
			case NL_ACT:
				num = (ye - ys) * 365;
				if (me == 2 && de == 29) {
					de = 28;
				}
				if (ms == 2 && ds == 29) {
					ds = 28;
				}
				num += (Date.getValueOf(de, me, 1997).getSerial() - Date.getValueOf(ds, ms, 1997).getSerial());
				break;
			case ACT_ACT:
			case ACT_360:                      // u ACT je to nejjednodussi, staci
			case ACT_365:                      // odecist seriovy cisla
				num = dEnd.getSerial() - dStart.getSerial();
				break;
			default:
				throw new IllegalArgumentException("Daycount convention " + convention +
						" not recognized.");
		}
		return sign * num;
	}

	/**
	 * Function Denum return number of days in a given year mainly for
	 * purposes of YearFrac calculactions.
	 * @param start
	 * @param end
	 * @param convention
	 * @return Function Denum returns number of days in a year representing
	 * the period given by <code>start</code> and <code>end</code> dates.
	 * Function Denum returns an annual representation, function
	 * {@link CouponDenum} returns just the fraction of the year.
	 * Always returns a positive number, that is if
	 * necessary, start and end dates are switched.
	 */
	public static double Denum(Date start, Date end, DaycountConvention convention) {

		Date dStart, dEnd;
		if (start.getSerial() > end.getSerial()) {
			dStart = end;
			dEnd = start;
		} else {
			dStart = start;
			dEnd = end;
		}

		double denum = 0.0;

		// NON_EOM modifier doesn't need to be taken into account,
		// we'll modify the convention...
		DaycountConvention conventionMod = DaycountConvention.toDaycountConventionEOM(convention);
		switch (conventionMod) {
			case US_30_360:
			case ISMA_30_360:
			case ACT_360:
			case NL_360:
				denum = 360;
				break;
			case US_30_365:
			case ISMA_30_365:
			case ACT_365:
			case NL_365:
				denum = 365;
				break;
			case US_30_ACT:
			case ISMA_30_ACT:
			case ACT_ACT:
			case NL_ACT:
				Date dt1;
				Date dt2;

				int ndays;
				int yearend;

				int ds = dStart.getDay();
				int ms = dStart.getMonth();
				int ys = dStart.getYear();
				int de = dEnd.getDay();
				int me = dEnd.getMonth();
				int ye = dEnd.getYear();

				int ss = dStart.getSerial();
				int se = dEnd.getSerial();

				switch (DaycountConvention.toDaycountConventionEOM(conventionMod)) {
					case US_30_ACT:
						if (de == 31) {
							de = 30;
						}
						if (dStart.isMonthEnd() || ds == 30) {
							ds = 1;
							ms++;
							if (ms == 13) {
								ms = 1;
								ys++;
							}
						}
						break;
					case ISMA_30_ACT:
						if (ds == 31) {
							ds = 30;
						}
						if (de == 31) {
							de = 30;
						}
						break;
					case NL_ACT:
						if (me == 2 && de == 29) {
							de = 28;
						}
						if (ms == 2 && ds == 29) {
							ds = 28;
						}
						break;
				}

				if (dStart.isLeap()) // zacina obdobi v prestupnem roce?
				{
					dt1 = Date.getValueOf(29, 2, ys);            // obsahuje obdobi den 29.2.?
					if (dt1.getSerial() >= ss && dt1.getSerial() <= se) {
						ndays = 366;
					} // ano...proto je nutne vzit 366 dnu
					else {
						ndays = 365;
					}                   // je to sice prestupny rok, ale
				// 29.2.byl uz pred zacatkem obdobi
				} else {
					ndays = 365;
				}                     // neprestupny rok, proto 365

				dt2 = Date.getValueOf(31, 12, ys);
				yearend = dt2.getSerial();
				if (se <= yearend) {
					denum = ndays;
				} // cele obdobi lezi v jednom kalendarnim
				else // roce
				{
					denum = ((double) (yearend - ss)) / ndays;

					if (dEnd.isLeap()) // konci obdobi v prestupnem roce?
					{
						dt1 = Date.getValueOf(29, 2, ye);          // obsahuje obdobi den 29.2.?
						if (dt1.getSerial() <= se) {
							ndays = 366;
						} else {
							ndays = 365;
						}
					} else {
						ndays = 365;
					}

					dt2 = Date.getValueOf(31, 12, ye - 1);
					denum += ((double) (se - dt2.getSerial())) / ndays + (ye - ys - 1);
					denum = ((double) (se - ss)) / (denum);
				}

				break;
			default:
				throw new IllegalArgumentException("Daycount convention " + convention +
						" not recognized.");
		}
		return denum;
	}

	/**
	 * Function CouponDenum returns number of days in the coupon period for
	 * purposes of accrued interest and coupon payment calculations.
	 * @param start start of the coupon period
	 * @param end end of the coupon period
	 * @param frequency annual coupon frequency
	 * @param convention daycount convention
	 * @return Function CouponDenum serves a similar purpose as function {@link Denum},
	 * with one important difference - where Denum returns number of days in a
	 * "yearly" coupon period and hence it is suitable for instance for {@link YearFrac}
	 * calculation, CouponDenum returns number of days in the the coupon period
	 * defined by <code>start</code> and <code>end</code> parameters. That means
	 * that for a seminnaul bond CouponDenum return approximately 180 - 183 days,
	 * meanwhile Denum would return 360 - 366 days, depending on the dates and
	 * the daycount convention. Always returns a positive number, that is if
	 * necessary, start and end dates are switched.
	 */
	public static double CouponDenum(Date start, Date end, int frequency, DaycountConvention convention) {
		double denum;
		DaycountConvention conventionMod = DaycountConvention.toDaycountConventionEOM(convention);
		switch (conventionMod) {
			case US_30_360:
			case ISMA_30_360:
			case ACT_360:
			case NL_360:
				denum = 360.0 / frequency;
				break;
			case US_30_365:
			case ISMA_30_365:
			case ACT_365:
			case NL_365:
				denum = 365.0 / frequency;
				break;
			case US_30_ACT:
			case ISMA_30_ACT:
			case ACT_ACT:
			case NL_ACT:
				denum = Math.abs(end.toInt() - start.toInt());
				break;
			default:
				throw new IllegalArgumentException("Daycount convention not recognized: " +
						convention);
		}
		return denum;
	}
}

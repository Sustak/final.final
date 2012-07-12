/*
 *  Finale, Copyright (c) 2008 Marek Šesták
 *  This programm has been released under GNU GPL v2.0
 */
package finale;

import finale.utils.Date;
import static finale.Utils.*;

/**
 *
 * @author Šesták
 */
public class Coupons {

	/**
	 * Function getNCoupons returns number of coupon payments between
	 * settlement date and maturity date.
	 * @param settlement Settlement date
	 * @param maturity Maturity date
	 * @param frequency Coupon frequency (1=annual, 2=semiannual, 3=triyearly, 4=quaterly, 6=bimonthly, 12=monthly)
	 * @param convention Daycount convetion
	 * @return Returns the number of coupon payments between settlement date and
	 * maturity date. For dates past the maturity date, 0 is returned.
	 * @throws IllegalArgumentException for illegal values of parameter frequency.
	 */
	public static int getNCoupons(Date settlement, Date maturity, int frequency, DaycountConvention convention) {
		int result;
		int nmonths;

		if (frequency != 1 && frequency != 2 && frequency != 3 && frequency != 4 &&
				frequency != 6 && frequency != 12) {
			throw new IllegalArgumentException("Illegal frequency: " + frequency);
		}

		if (settlement.toInt() >= maturity.toInt()) {
			return 0;
		}

		nmonths = (maturity.getYear() - settlement.getYear()) * 12 + (maturity.getMonth() - settlement.getMonth());

		if (convention.isNonEOM()) {
			if (settlement.getDay() >= maturity.getDay()) {
				nmonths--;
			} else if (settlement.isMonthEnd()) {
				nmonths--;
			}
		} else {
			if (maturity.isMonthEnd() && settlement.isMonthEnd()) {
				nmonths--;
			} else {
				if (maturity.isMonthEnd() == false) {
					if (settlement.getDay() >= maturity.getDay()) {
						nmonths--;
					} else {
						if (settlement.isMonthEnd()) {
							nmonths--;
						}
					}
				}
			}
		}

		result = nmonths / (12 / frequency) + 1;

		return result;
	}

	public static boolean isCouponDate(Date settlementDate, Date maturity,
			int frequency, DaycountConvention convention) {
		return Coupons.getPrevCouponDate(settlementDate, maturity, frequency, convention).
				equals(settlementDate);
	}

	public static Date getPrevCouponDate(Date settlement, Date maturity,
			int frequency, DaycountConvention convention) {

		Date result;
		int ncpn;

		if (frequency != 1 && frequency != 2 && frequency != 3 && frequency != 4 &&
				frequency != 6 && frequency != 12) {
			throw new IllegalArgumentException("Illegal frequency: " + frequency);
		}

		if (settlement.toInt() > maturity.toInt()) {
			return maturity;
		// ??? shouldn't we rather throw an exception?
		}

		if (settlement == maturity) {
			return maturity;
		}

		ncpn = getNCoupons(settlement, maturity, frequency, convention) - 1;
		result = maturity.addMonths(-(ncpn + 1) * (12 / frequency));

		if (!convention.isNonEOM()) {
			if (maturity.isMonthEnd()) {
				return result.endOfMonth(0);
			}
		}

		return result;
	}

	static Date getNextCouponDateNoChecks(Date settlement, Date maturity, int frequency,
			DaycountConvention convention) {

		int ncpn;
		if( settlement.toInt()<=maturity.toInt() )
			ncpn = Coupons.getNCoupons(settlement, maturity, frequency, convention) - 1;
		else
			ncpn = - Coupons.getNCoupons(maturity, settlement, frequency, convention) - 1;

		Date result = maturity.addMonths(-(ncpn) * (12 / frequency));

		if (!convention.isNonEOM()) {
			if (maturity.isMonthEnd()) {
				return result.endOfMonth(0);
			}
		}

		return result;

	}

	public static Date getNextCouponDate(Date settlement, Date maturity, int frequency,
			DaycountConvention convention) {

		Date result;
		int ncpn;

		if (frequency != 1 && frequency != 2 && frequency != 3 && frequency != 4 && frequency != 6 && frequency != 12) {
			throw new IllegalArgumentException("Illegal frequency: " + frequency);
		}

		if (settlement.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("No coupons after maturity (settlement " +
					settlement + ", maturity " + maturity);
		}

		return getNextCouponDateNoChecks(settlement, maturity, frequency, convention);
	}

	/**
	 * Function getCouponDaysBS returns number of days from the beginning of the
	 * coupon period to the settlement date.
	 * @param settlement security's settlement date
	 * @param maturity security's final maturity date
	 * @param frequency security's coupon frequency (number of coupon payments per year)
	 * @param convention security's daycount convention
	 * @return number of days from settlement date to the maturity date
	 * with regard to the daycount convention and frequency of a bond.
	 */
	public static int getCouponDaysBS(Date settlement, Date maturity,
			int frequency, DaycountConvention convention) {
		Date beginning = Coupons.getPrevCouponDate(settlement, maturity, frequency, convention);
		return NDays(beginning, settlement, convention);
	}

	public static int getCouponDays(Date settlement, Date amaturity,
			int frequency, DaycountConvention convention) {
		Date beginning, next;
		beginning = getPrevCouponDate(settlement, amaturity, frequency, convention);
		next = getNextCouponDate(settlement, amaturity, frequency, convention);
		return NDays(beginning, next, convention);
	}

	public static int getCouponDaysNC(Date settlement, Date maturity,
			int frequency, DaycountConvention convention) {
		Date next;
		next = getNextCouponDate(settlement, maturity, frequency, convention);
		return NDays(settlement, next, convention);
	}

	/**
	 * Function getCouponPaymentSimple returns the coupon payment that a bond
	 * with given parameters would pay out on a given couponDate.
	 * @param couponDate date for which the coupon payment is returned
	 * @param maturity security's maturity date
	 * @param coupon annual security's coupon rate (expressed in decimal, that is for 3% the rate should be provided as 0.03)
	 * @param nominalAmount security's nominal amount
	 * @param frequency security's annual frequency at which coupons are paid out
	 * @param convention security's daycount convention used in coupon and/or accrued interest calculations
	 * @return The function returns an amount that the bond specified by the provided
	 *         maturity date, frequency of the coupon and the annual coupon rate
	 *         would be paid out on the coupon date.
	 * @throws For any date after the maturity date an <code>IllegalArgumentException</code>
	 *         is thrown.
	 */
	public static double getCouponPaymentSimple(Date couponDate, Date maturity,
			double coupon, double nominalAmount, int frequency,
			DaycountConvention convention) {

		if (couponDate.toInt() > maturity.toInt()) {
			throw new IllegalArgumentException("Coupon date (" + couponDate +
					") after security's maturity (" + maturity);
		}
		Date prevCoupon = Coupons.getPrevCouponDate(couponDate, maturity, frequency, convention);
		if (!prevCoupon.equals(couponDate)) {
			// TODO Shall we a] throw an exception and won't allow for it at all
			//            or b] return accrued interestr from prevCoupon to couponDate?
			throw new RuntimeException("not implemented");
		}
		return nominalAmount * coupon / frequency;
	}

	/** Function getCouponPayment returns the coupon payment that a bond
	 * with given parameters would pay out on a given couponDate.
	 * @param couponDate date for which the coupon payment is returned
	 * @param issueDate security's issue date (sometimes also refered to as first settlement date)
	 * @param maturity security's maturity date
	 * @param interestAccruesFrom date on which the security's coupon starts to accrue interest
	 * @param firstCouponDate date on which the security pays out the first coupon
	 * @param coupon annual security's coupon rate (expressed in decimal, that is for 3% the rate should be provided as 0.03)
	 * @param nominalAmount security's nominal amount
	 * @param frequency security's annual frequency at which coupons are paid out
	 * @param convention security's daycount convention used in coupon and/or accrued interest calculations
	 * @return The function returns an amount that the bond specified by the provided
	 *         maturity date, frequency of the coupon and the annual coupon rate
	 *         would be paid out on the coupon date.
	 * @throws For any non-coupon date an <code>IllegalArgumentException</code>
	 *         is thrown. The same exception is also thrown for invalid combinations of
	 *         issueDate, interestAccruesFrom, firstCouponDate and maturity parameters.
	 */
	public static double getCouponPayment(Date couponDate, Date issueDate, Date maturity,
			Date interestAccruesFrom, Date firstCouponDate,
			double coupon, double nominalAmount, int frequency,
			DaycountConvention convention) {

		if (!Coupons.isCouponDate(firstCouponDate, maturity, frequency, convention)) {
			throw new IllegalArgumentException("Provided first coupon date must be an actual coupon date " +
					"(first coupon date: " + firstCouponDate + ")");
		}

		if (couponDate.toInt() > firstCouponDate.toInt()) {
			return Coupons.getCouponPaymentSimple(couponDate, maturity, coupon, nominalAmount, frequency, convention);
		}

		if (couponDate.toInt() < issueDate.toInt()) {
			throw new IllegalArgumentException("Invalid coupon date (" + couponDate + ")");
		}

		if (issueDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Issue date must be before the maturity date " +
					"(issue date: " + issueDate + ", maturity date: " + maturity + ")");
		}

		if (interestAccruesFrom.toInt() >= firstCouponDate.toInt()) {
			throw new IllegalArgumentException("Date from which the coupon starts to accrue must be " +
					"before the first coupon is payed out (interest accrues from: " + interestAccruesFrom +
					", first coupon date: " + firstCouponDate + ")");
		}

		if (firstCouponDate.toInt() > maturity.toInt()) {
			throw new IllegalArgumentException("First coupon date must be on or before the maturity date " +
					"(first coupon date: " + firstCouponDate + ", maturity date: " + maturity + ")");
		}

		if (couponDate.equals(firstCouponDate)) {

			Date cpnBeforeIntAccr = Coupons.getPrevCouponDate(interestAccruesFrom, maturity, frequency, convention);

			if (interestAccruesFrom.equals(cpnBeforeIntAccr)) {
				return nominalAmount * coupon / frequency * Coupons.getNCoupons(interestAccruesFrom, firstCouponDate, frequency, convention);
			}

			Date cpnAfterIntAccr = Coupons.getNextCouponDate(interestAccruesFrom, maturity, frequency, convention);

			double accrued = nominalAmount * coupon / frequency *
					NDays(interestAccruesFrom, cpnAfterIntAccr, convention) /
					CouponDenum(cpnBeforeIntAccr, cpnAfterIntAccr, frequency, convention);

			accrued += nominalAmount * coupon / frequency *
					getNCoupons(cpnAfterIntAccr, firstCouponDate, frequency, convention);

			return accrued;

		} else {

			throw new IllegalArgumentException("No coupon payment on the given date: " + couponDate);
		}
	}

	public static double getAccruedInterestSimple(Date settlementDate, Date maturity,
			double coupon, double nominalAmount, int frequency,
			DaycountConvention convention) {

		if (Date.compare(settlementDate, maturity) > 0) {
			throw new IllegalArgumentException("Accrued interest cannot be calculated for settlement dates " +
					"past the maturity date (settlement date: " + settlementDate +
					", maturity date: " + maturity + ")");
		}
		if (settlementDate.equals(maturity)) {
			return 0.0;
		}
		Date prevCpn = Coupons.getPrevCouponDate(settlementDate, maturity, frequency, convention);
		Date nextCpn = Coupons.getNextCouponDate(settlementDate, maturity, frequency, convention);
		double denum = CouponDenum(prevCpn, nextCpn, frequency, convention);
		int num = NDays(prevCpn, settlementDate, convention);
		return nominalAmount * coupon / frequency * num / denum;
	}

	public static double getAccruedInterest(Date settlementDate, Date issueDate, Date maturity,
			Date interestAccruesFrom, Date firstCouponDate,
			double coupon, double nominalAmount, int frequency,
			DaycountConvention convention) {

		if (coupon == 0.0) {
			return 0.0;
		}

		if (!Coupons.isCouponDate(firstCouponDate, maturity, frequency, convention)) {
			throw new IllegalArgumentException("Provided first coupon date must be an actual coupon date " +
					"(first coupon date: " + firstCouponDate + ")");
		}

		if (settlementDate.toInt() > firstCouponDate.toInt()) {
			return Coupons.getAccruedInterestSimple(settlementDate, maturity, coupon, nominalAmount, frequency, convention);
		}

		if (settlementDate.equals(maturity) || Date.compare(settlementDate, interestAccruesFrom) <= 0) {
			return 0.0;
		}

		if (issueDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Issue date must be before the maturity date " +
					"(issue date: " + issueDate + ", maturity date: " + maturity + ")");
		}

		if (interestAccruesFrom.toInt() >= firstCouponDate.toInt()) {
			throw new IllegalArgumentException("Date from which the coupon starts to accrue must be " +
					"before the first coupon is payed out (interest accrues from: " + interestAccruesFrom +
					", first coupon date: " + firstCouponDate + ")");
		}

		if (firstCouponDate.toInt() > maturity.toInt()) {
			throw new IllegalArgumentException("First coupon date must be on or before the maturity date " +
					"(first coupon date: " + firstCouponDate + ", maturity date: " + maturity + ")");
		}

		// all coupon dates before first coupon date and all after maturity
		// have been answered by now...

		Date cpnAfterIntaccr = Coupons.getNextCouponDate(interestAccruesFrom, maturity, frequency, convention);
		Date cpnBeforeIntaccr = Coupons.getPrevCouponDate(interestAccruesFrom, maturity, frequency, convention);

		if (Date.compare(settlementDate, cpnAfterIntaccr) <= 0) {
			return nominalAmount * coupon / frequency *
					(double) NDays(interestAccruesFrom, settlementDate, convention) /
					CouponDenum(cpnBeforeIntaccr, cpnAfterIntaccr, frequency, convention);
		}

		double accrued = nominalAmount * coupon / frequency *
				NDays(interestAccruesFrom, cpnAfterIntaccr, convention) /
				CouponDenum(cpnBeforeIntaccr, cpnAfterIntaccr, frequency, convention);

		Date cpndt_before_settle = Coupons.getPrevCouponDate(settlementDate, maturity, frequency, convention);
		Date cpndt_after_settle = Coupons.getNextCouponDate(settlementDate, maturity, frequency, convention);

		accrued += nominalAmount * coupon / frequency * Coupons.getNCoupons(cpnAfterIntaccr, cpndt_before_settle, frequency, convention);
		accrued += nominalAmount * coupon / frequency *
				NDays(cpndt_before_settle, settlementDate, convention) /
				CouponDenum(cpndt_before_settle, cpndt_after_settle, frequency, convention);

		return accrued;
	}

	@Deprecated
	public static int CouponNum(Date settlement, Date maturity, int frequency, DaycountConvention convention) {
		return Coupons.getNCoupons(settlement, maturity, frequency, convention);
	}
}

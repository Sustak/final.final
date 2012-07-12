package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import java.util.ArrayList;
import static finale.utils.Math.*;
import static finale.Utils.*;

/**
 * <ul>
 * <li>Used for Japanese government bonds which have either a normal or a
 * short last coupon period.</li>
 * <li>JGBs issued prior to March 1, 2001 include both start and end dates for
 * the first coupon period.</li>
 * <li>Bonds issued after March 1, 2001 have a full semi-annual coupon
 * payment at the first coupon date regardless of the issue date.
 * The interest accrued period always starts six months prior to the first
 * coupon payment date.</li>
 * <li>Accrued includes February 29, while simple yield does not unless
 * settled on February 29 or if there is less than one year to maturity.</li>
 * <li>Settlement is T+3.</li>
 * </ul>
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondJGB extends Bond {

	BondJGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public String getBondType() {
		return "Japanese government bond (JGB)";
	}

	/**
	 * BondJGB.createCashflows creates cashflow for Japanese governement
	 * bonds. The way in which cashflows of these bonds are calculated
	 * are slightly different from normal generic bonds. All JGBs are semiannual
	 * and most of them mature on the 20th day of a month.
	 * <ul>
	 * <li>Bonds issued prior to March 1 2001 typically have "long first
	 * coupon period" which includes both starting and ending days of the period.
	 * That means, if a bond with 2% coupon is issued on Jan-1 and first coupon is 
	 * paid out on August-20,
	 * to calculate first coupon amount one has to calculate accrued
	 * interest from Jan-1 to Feb-20 (that would normally be first coupon date),
	 * that is ((#Feb-20#-#Jan-1#)+1)/365*0.02 = ((31)+1)/365*0.02 and
	 * add 1% (2%/2).
	 * Please note the '+1' in the expression above - 
	 * that extra day is added precisely because
	 * the ending day of the first coupon period is included as well
	 * (this is not the case for standard bonds).<li>
	 * </ul>
	 * @return Created array of cashflows is returned.
	 */
	@Override
	Cashflow[] createCashflows() {

		ArrayList<Cashflow> cshf = new ArrayList<Cashflow>();

		double accrued = 0.0;
		Date interestTo = interestAccruesFrom;

		if (Coupons.getPrevCouponDate(interestAccruesFrom, firstCouponDate, frequency, convention).toInt() < interestAccruesFrom.toInt()) {

			// first coupon period doesn't start on coupon date
			// accrued interest has to be calculated separately

			interestTo = Coupons.getNextCouponDateNoChecks(issueDate, firstCouponDate, frequency, convention);
			int ndays = interestTo.toInt() - interestAccruesFrom.toInt();

			// as noted above, JGBs issued before March 1 2001 accrue
			// interest in the first coupon period both on first and last day
			// of the period (hence number of days has to increased by 1)
			if (issueDate.toInt() <= Date.getSerial(1, 3, 2001)) {
				ndays++;
			}

			accrued = RoundDown((double) ndays / 365.0 * coupon * 1000000.0, 0) / 10000.0;

		}

		double standardCouponAmount = RoundDown(coupon / frequency * 1000000.0, 0) / 10000.0;
		accrued += standardCouponAmount * Coupons.getNCoupons(interestTo, firstCouponDate, frequency, convention);
		cshf.add(new Cashflow(firstCouponDate, accrued));

		interestTo = Coupons.getNextCouponDateNoChecks(firstCouponDate, firstCouponDate, frequency, convention);
		accrued = 0.0;
		double principal = 0.0;

		while (interestTo.toInt() <= maturity.toInt()) {

			int nDaysRemaining = maturity.toInt() - interestTo.toInt();
			if (nDaysRemaining < 7) {
				accrued = RoundDown((double) nDaysRemaining / 365.0 * 1000000.0 * coupon, 0) / 10000.0;
				principal = redemption;
				interestTo = maturity;
			}
			cshf.add(new Cashflow(interestTo, principal + standardCouponAmount + accrued));
			accrued = 0.0;

			interestTo = Coupons.getNextCouponDateNoChecks(interestTo, firstCouponDate, frequency, convention);
		}

		return (Cashflow[]) cshf.toArray(new Cashflow[cshf.size()]);
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {

		int settleInt = settlementDate.toInt();

		if (settleInt < issueDate.toInt() || settleInt >= maturity.toInt()) {
			return 0.0;
		}

		if (settleInt < firstCouponDate.toInt()) {
			double ndays = settlementDate.toInt() - interestAccruesFrom.toInt();
			if (issueDate.toInt() < Date.getSerial(1, 3, 2001)) {
				ndays++;
			}
			return RoundDown(coupon * 100.0 * ndays / 365.0, 7);
		}

		return RoundDown(coupon * 100.0 / 365.0 * (settleInt - getPrevCouponDate(settlementDate).toInt()), 7);
	}

	@Override
	public double getConventionalPrice(Date settlementDate, double yield) {

		return RoundDown(getConventionalPriceUnrounded(settlementDate, yield), 3);

	}

	@Override
	public double getConventionalYield(Date settlementDate, double price) {

		return RoundDown(getConventionalYieldUnrounded(settlementDate, price), 5 );

	}

	@Override
	public double getConventionalPriceUnrounded(Date settlementDate, double yield) {

		return getJapaneseSimplePrice(settlementDate, yield, coupon, maturity, redemption);

	}

	@Override
	public double getConventionalYieldUnrounded(Date settlementDate, double price) {

		return getJapaneseSimpleYield(settlementDate, price, coupon, maturity, redemption);

	}

	public static double getJapaneseSimpleYield(Date settlementDate, double price,
			double coupon, Date maturity, double redemption) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Yield cannot be calculated for" +
					" dates past the maturity date (settlement: " +
					settlementDate.toString() + ", maturity: " + maturity.toString() + ")");
		}

		double ndays = NDays(settlementDate, maturity, DaycountConvention.NL_365);
		return (coupon * 100.0 + (redemption - price) / ndays * 365.0) / price;
	}

	public static double getJapaneseSimplePrice(Date settlementDate, double yield,
			double coupon, Date maturity, double redemption) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Price cannot be calculated for" +
					" dates past the maturity date (settlement: " +
					settlementDate.toString() + ", maturity: " + maturity.toString() + ")");
		}

		double ndays = NDays(settlementDate, maturity, DaycountConvention.NL_365);
		return (coupon * 100.0 + redemption / (ndays/365)) / (yield + 1.0 / (ndays/365));
	}
}

package finale;

import finale.utils.Date;
import finale.utils.Estimator;
import finale.utils.EstimatorLinear;
import java.util.ArrayList;
import java.util.Arrays;
import static finale.Utils.*;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class Bond implements Instrument, HasCashflows, DebtSecurity {

	private static Calendar calendarDefault = Calendar.getCalendar("");

	public Calendar getCalendar() {
		return calendarDefault;
	}

	public static class Builder {

		public Builder() {
		}

		public Builder(double coupon, Date maturity, Date issueDate) {

			setCoupon(coupon);
			setMaturity(maturity);
			setIssueDate(issueDate);
		}

		public Builder(double coupon, Date maturity, Date issueDate,
				int frequency) {

			setCoupon(coupon);
			setMaturity(maturity);
			setIssueDate(issueDate);
			setFrequency(frequency);
		}

		public Builder(double coupon, Date maturity, Date issueDate,
				int frequency, DaycountConvention convention) {

			setCoupon(coupon);
			setMaturity(maturity);
			setIssueDate(issueDate);
			setFrequency(frequency);
			setDaycountConvention(convention);
		}

		public Builder(double coupon, Date maturity, Date issueDate,
				int frequency, DaycountConvention convention,
				double redemption, Date interestAccruesFrom,
				Date firstCouponDate) {

			setCoupon(coupon);
			setMaturity(maturity);
			setIssueDate(issueDate);
			setFrequency(frequency);
			setDaycountConvention(convention);
			setRedemption(redemption);
			setInterestAccruesFrom(interestAccruesFrom);
			setFirstCouponDate(firstCouponDate);
		}

		public Builder setTicker(String ticker) {
			this.ticker = ticker;
			tickerSet = true;
			return this;
		}
		String ticker;
		boolean tickerSet = true;

		public Builder setIssueDate(Date issueDate) {
			this.issueDate = issueDate;
			issueDateSet = true;
			return this;
		}
		Date issueDate;
		boolean issueDateSet = false;

		public Builder setMaturity(Date maturity) {
			this.maturity = maturity;
			maturitySet = true;
			return this;
		}
		Date maturity;
		boolean maturitySet = false;

		public Builder setInterestAccruesFrom(Date interestAccruesFrom) {
			this.interestAccruesFrom = interestAccruesFrom;
			interestAccruesFromSet = true;
			return this;
		}
		Date interestAccruesFrom;
		boolean interestAccruesFromSet = false;

		public Builder setFirstCouponDate(Date firstCouponDate) {
			this.firstCouponDate = firstCouponDate;
			firstCouponDateSet = true;
			return this;
		}
		Date firstCouponDate;
		boolean firstCouponDateSet = false;

		public Builder setCoupon(double coupon) {
			this.coupon = coupon;
			couponSet = false;
			return this;
		}
		double coupon;
		boolean couponSet = false;

		public Builder setFrequency(int frequency) {
			this.frequency = frequency;
			frequencySet = true;
			return this;
		}
		int frequency;
		boolean frequencySet = false;

		public Builder setDaycountConvention(DaycountConvention convention) {
			this.convention = convention;
			conventionSet = true;
			return this;
		}

		public Builder setDaycountConvention(String convention) {
			this.convention = DaycountConvention.toDaycountConvention(convention);
			conventionSet = true;
			return this;
		}
		DaycountConvention convention;
		boolean conventionSet = false;

		public Builder setRedemption(double redemption) {
			this.redemption = redemption;
			redemptionSet = true;
			return this;
		}
		double redemption;
		boolean redemptionSet = false;

		public Builder setCompoundingFrequency(int compoundingFrequency) {
			this.compoundingFrequency = compoundingFrequency;
			this.compoundingFrequencySet = true;
			return this;
		}
		int compoundingFrequency;
		boolean compoundingFrequencySet = false;

		public Builder setTradesExDividend(boolean tradesExDividend) {
			this.tradesExDividend = tradesExDividend;
			this.tradesExDividendSet = true;
			return this;
		}
		boolean tradesExDividend;
		boolean tradesExDividendSet = false;

		public Builder setExDividendDays(int exDividendDays) {
			if (exDividendDays > 0) {
				this.exDividendDays = exDividendDays;
				this.exDividendDaysSet = true;
				setTradesExDividend(true);
			}
			return this;
		}
		int exDividendDays;
		boolean exDividendDaysSet = false;
	};
	Date issueDate;
	Date interestAccruesFrom;
	Date maturity;
	Date firstCouponDate;
	double coupon;
	DaycountConvention convention;
	int frequency;
	int compoundingFrequency;
	double redemption;
	String ticker;
	boolean isOddCoupon;
	boolean tradesExDividend;
	int exDividendDays;
	private Cashflow[] cashflows;
	public static final double YIELD_PRECISION = 1e-12;

	public DaycountConvention getConvention() {
		return convention;
	}

	public double getCoupon() {
		return coupon;
	}

	public Date getFirstCouponDate() {
		return firstCouponDate;
	}

	public int getFrequency() {
		return frequency;
	}

	public int getCompoundingFrequency() {
		return compoundingFrequency;
	}

	public Date getInterestAccruesFrom() {
		return interestAccruesFrom;
	}

	public Date getIssueDate() {
		return issueDate;
	}

	public Date getMaturity() {
		return maturity;
	}

	public double getRedemption() {
		return redemption;
	}

	public boolean getTradesExDividend() {
		return tradesExDividend;
	}

	/**
	 * Private default constructor, all instances of this class
	 * should be acquired through getBond.... static factories.
	 */
	private Bond() {
		throw new RuntimeException("Class Bond: default constructor invoked");
	}

	Bond(Builder parameters) {
		updateBond(parameters);
	}

	public void updateBond(Builder parameters) {
		ticker = parameters.ticker;
		issueDate = parameters.issueDate;
		maturity = parameters.maturity;
		interestAccruesFrom = parameters.interestAccruesFrom;
		firstCouponDate = parameters.firstCouponDate;
		coupon = parameters.coupon;
		frequency = parameters.frequency;
		compoundingFrequency = parameters.compoundingFrequency;
		convention = parameters.convention;
		redemption = parameters.redemption;
		isOddCoupon = !Coupons.isCouponDate(firstCouponDate, maturity, frequency, convention);
		tradesExDividend = parameters.tradesExDividend;
		exDividendDays = parameters.exDividendDays;
		cashflows = createCashflows();
	}

	Cashflow[] createCashflows() {

		ArrayList<Cashflow> cshf = new ArrayList<Cashflow>();

		Date interestFrom = interestAccruesFrom;
		Date interestTo = firstCouponDate;
		double amount;

		if (Coupons.isCouponDate(firstCouponDate, maturity, frequency, convention)) {

			// we have to create cashflows for a standard bond...

			while (interestTo.toInt() <= maturity.toInt()) {
				amount = Coupons.getCouponPayment(interestTo,
						issueDate, maturity, interestAccruesFrom, firstCouponDate,
						coupon, 100.0, frequency, convention);
				if (interestTo.toInt() == maturity.toInt()) {
					amount += redemption;
				}
				if (amount > 0) {
					cshf.add(new Cashflow(interestTo, amount));
				}
				if (maturity.toInt() == interestTo.toInt()) {
					break;
				}
				interestTo = Coupons.getNextCouponDate(interestTo, maturity, frequency, convention);
			}

		} else {

			// a special odd-coupon-bond
			// TODO: proper testing is required against some odd-coupon bond from bloomberg!

			while (interestTo.toInt() <= maturity.toInt()) {
				amount = YearFrac(interestFrom, interestTo, convention) *
						coupon / frequency * 100.0;
				if (interestTo.toInt() == maturity.toInt()) {
					amount += redemption;
				}
				if (amount > 0) {
					cshf.add(new Cashflow(interestTo, amount));
				}

				if (interestTo.equals(maturity)) {
					break;
				}

				interestFrom = interestTo;
				interestTo = Date.EDate(interestTo, 12 / frequency);
				if (interestTo.toInt() > maturity.toInt()) {
					interestTo = maturity;
				}
			}

		}

		return (Cashflow[]) cshf.toArray(new Cashflow[cshf.size()]);
	}

	/**
	 * Function getBond is a static factory that creates new instances
	 * of class Bond.
	 *
	 * @param parameters the function takes the only parameter, and that is
	 * an instance of class {@link Builder}
	 *
	 * @return Function returns an instance of the class Bond with characteristics
	 * provided in the parameters. The factory employs the following default
	 * conversions for missing parameter fields:
	 * <ul>
	 * <li>default daycount convention is ACT/ACT </li>
	 * <li>default coupon payment freqyency is 1 </li>
	 * <li>if interestAccruesFrom is missing then a date of theoretical coupon
	 * preceeding the issue date is used </li>
	 * <li>if firstCouponDate is missing then a date of first regular coupon
	 * following the issue date is used </li>
	 * <li>default redemption amount is 100, or "par"</li>
	 * <li>default compoundingFrequency is equal to coupon frequency
	 * (that means if both are missing, 1 is used for both of them)</li>
	 * </ul>
	 *
	 * @throws IllegalArgumentException when:
	 * <ul>
	 * <li>parameters issueDate or maturity are missing</li>
	 * <li>parameter issueDate is on or afer maturity</li>
	 * <li>parameter firstCouponDate is after maturity</li>
	 * <li>parameter interestAccruesFrom is after maturity</li>
	 * <li>parameter interestAccruesFrom is after firstCouponDate</li>
	 * <li>parameter coupon or redemption are smaller than zero</li>
	 * <li>parameter frequency isn't one of the following values:
	 * 1,2,3,4,6,12</li>
	 * </ul>
	 *
	 */
	public static Bond getBond(Builder parameters) {
		if (parameters.issueDate == null) {
			throw new IllegalArgumentException("Issue date not initialized");
		}
		if (parameters.maturity == null) {
			throw new IllegalArgumentException("Maturity date not initiliazed");
		}
		if (parameters.issueDate.toInt() >= parameters.maturity.toInt()) {
			throw new IllegalArgumentException("Issue date must be before maturity");
		}
		if (parameters.convention == null) {
			parameters.convention = DaycountConvention.ACT_ACT;
		}
		if (parameters.frequencySet == false) {
			parameters.frequency = 1;
		}
		if ((12 % parameters.frequency) != 0) {
			throw new IllegalArgumentException("Invalid coupon frequency (" +
					parameters.frequency + ")");
		}
		if (parameters.redemptionSet == false) {
			parameters.redemption = 100.0;
		}
		if (parameters.compoundingFrequencySet == false) {
			parameters.compoundingFrequency = parameters.frequency;
		}
		if ((12 % parameters.compoundingFrequency) != 0) {
			throw new IllegalArgumentException("Invalid compounding frequency (" +
					parameters.compoundingFrequency + ")");
		}
		if (parameters.firstCouponDate == null) {
			parameters.firstCouponDate = Coupons.getNextCouponDate(
					parameters.issueDate, parameters.maturity,
					parameters.frequency, parameters.convention);
		}
		if (parameters.interestAccruesFrom == null) {
			parameters.interestAccruesFrom = Coupons.getPrevCouponDate(
					parameters.issueDate, parameters.maturity,
					parameters.frequency, parameters.convention);
		}
		if (parameters.interestAccruesFrom.toInt() >= parameters.maturity.toInt()) {
			throw new IllegalArgumentException(
					"Date, from which the interest starts to accrue " +
					"has to be before maturity");
		}
		if (parameters.firstCouponDate.toInt() > parameters.maturity.toInt()) {
			throw new IllegalArgumentException(
					"First coupon date has to be on or before maturity");
		}
		if (parameters.firstCouponDate.toInt() <= parameters.interestAccruesFrom.toInt()) {
			throw new IllegalArgumentException(
					"Date from which interest starts to accrue has to be on or " +
					"before the first coupon is paid out");
		}
		if (parameters.coupon < 0) {
			throw new IllegalArgumentException("Invalid coupon rate (" +
					parameters.coupon + ")");
		}
		if (parameters.redemption < 0) {
			throw new IllegalArgumentException("Invalid redemption amount (" +
					parameters.redemption + ")");
		}
		if (parameters.ticker == null) {
			parameters.ticker = "";
		}

		if (parameters.ticker.equalsIgnoreCase("BTPS")) {
			return new BondBTPS(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("JGB")) {
			return new BondJGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("FRTR")) {
			return new BondFRTR(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("SPGB")) {
			return new BondSPGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("ACGB")) {
			return new BondACGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("NZGB")) {
			return new BondNZGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("CAN")) {
			return new BondCAN(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("CZGB")) {
			return new BondCZGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("HGB")) {
			return new BondHGB(parameters);
		} else if (parameters.ticker.equalsIgnoreCase("POLGB")) {
			return new BondPOLGB(parameters);
		} else {
			return new Bond(parameters);
		}
	}

	public InstrumentType getInstrumentType() {
		return InstrumentType.Bond;
	}

	@Override
	public String toString() {
		return getBondType() +
				", ticker " + ticker +
				", coupon " + String.valueOf(coupon) +
				", maturity " + maturity +
				", frequency " + frequency +
				", daycount convention " + convention +
				", issue date " + issueDate +
				", interest accrues from " + interestAccruesFrom +
				", first coupon date " + firstCouponDate +
				", redemption " + redemption;
	}

	public String getBondType() {
		return "Generic bond";
	}

	// TODO hash a equals! ale naimplementuju to, az bude mit Bond i ticker a typ bondu
	public double getMarketValue(double amount, Date valueDate, double price) {
		return getMarketValue(amount, valueDate, price, PriceConvention.Price);
	}

	public double getMarketValue(double amount, Date valueDate, double price, PriceConvention priceConvention) {
		throw new UnsupportedOperationException("Not supported yet.");
	}

	public Cashflow getCashflow(int index) {
		if (index < 0 || index >= cashflows.length) {
			throw new IllegalArgumentException("No cashflows with index " + index +
					" (" + toString() + ")");
		}
		return cashflows[index];
	}

	/**
	 * Returns cashflow that follows the settlement date.
	 * @param settlementDate settlement date
	 * @return Returns Cashflow object with the nearest cashflow that the
	 * bond pays out <b>after</b> the settlement date. That means if the bond
	 * has a cashflow taking place exactly on settlement date, following
	 * cashflow is returned. If there's no such cashflow, null is returned.
	 */
	public Cashflow getNextCashflow(Date settlementDate) {

		int idx = getCashflowIndex(Date.getValueOf(settlementDate.toInt() + 1));
		if (idx == -1) {
			return null;
		}

		// If the settlement date falls into ex-div period, following
		// cashflow should be returned.
		// If the settlement date falls into last ex-div period, then
		// just the principal payment should be returned
		// (i.e. new cashflow object would have to be created)

		if (isExDividend(settlementDate)) {
			if (idx == (cashflows.length - 1)) {
				return new Cashflow(maturity, redemption);
			}
			idx++;
		}

		return cashflows[idx];
	}

	/**
	 * Returns coupon date that follows the settlement date.
	 * @param settlementDate Settlement date
	 * @return Returns the nearest coupon date that follows the settlement date.
	 * If the bond
	 * has a cashflow taking place exactly on settlement date, the next coupon
	 * date is returned. For settlement dates preceding the date on which
	 * interest starts to accrue, the date on which interest starts to accrue is
	 * also returned. For settlement dates on or past maturity date,
	 * null is returned.
	 */
	public Date getNextCouponDate(Date settlementDate) {

		if (settlementDate.toInt() < interestAccruesFrom.toInt()) {
			return interestAccruesFrom;
		}

		if (settlementDate.toInt() >= maturity.toInt()) {
			return null;
		}

		Cashflow cf = getNextCashflow(settlementDate);
		return cf.getDate();
	}

	/**
	 * Returns cashflow that precedes the settlement date.
	 * @param settlementDate settlement date
	 * @return Returns Cashflow object with the nearest cashflow that the
	 * bond pays out <b>on</b> or <b>before</b>
	 * the settlement date. That means if the bond
	 * has a cashflow taking place exactly on settlement date, this
	 * cashflow is also returned. If there's no such cashflow
	 * (for instance for dates before the first coupon is paid out)
	 * null is returned.
	 */
	public Cashflow getPrevCashflow(Date settlementDate) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			return cashflows[cashflows.length - 1];
		}
		int idx = getCashflowIndex(Date.getValueOf(settlementDate.toInt() + 1));

		if (idx == -1) // this can only happen if the bond has no cashflows at all
		{
			return null;
		}

		if (idx == 0) // the next cashflow is the first one, that is there's no
		// preceding cashflow
		{
			return null;
		}

		return cashflows[idx - 1];
	}

	/**
	 * Returns previous coupon date that precedes the settlement date.
	 * @param settlementDate Settlement date
	 * @return Returns the nearest coupon date that precedes the settlement date.
	 * If the bond
	 * has a cashflow taking place exactly on settlement date, this
	 * date is also returned. For settlement dates preceding first coupon date,
	 * the date from which interest starts to accrue is returned - for all
	 * other preceding settlement dates, null is returned.
	 * For settlement dates after maturity, maturity is returned.
	 */
	public Date getPrevCouponDate(Date settlementDate) {

		if (settlementDate.toInt() <= firstCouponDate.toInt()) {
			if (settlementDate.toInt() < interestAccruesFrom.toInt()) {
				return null;
			} else {
				return interestAccruesFrom;
			}
		}

		Cashflow cf = getPrevCashflow(settlementDate);
		return cf.getDate();
	}

	/**
	 * Returns an array of all cashflows the bond pays out during its life.
	 * @return Returns an array of all cashflows the bond pays out during its
	 * whole life. The array is sorted by the dates on which the respective
	 * cashflows are paid out. The last cashflow includes both the last coupon
	 * and the principal.
	 */
	public Cashflow[] getCashflows() {
		return cashflows.clone();
	}

	/**
	 * Returns an array of all cashflows the bond pays out after the
	 * settlement date.
	 * @return Returns an array of all cashflows the bond pays out after
	 * whole life. The array is sorted by the dates on which the respective
	 * cashflows are paid out. The last cashflow includes both the last coupon
	 * and the principal.
	 */
	public Cashflow[] getCashflows(Date settlementDate) {

		int idx = getCashflowIndex(Date.getValueOf(settlementDate.toInt() + 1));
		if (idx == -1) {
			return null;
		}

		Cashflow[] cshf = Arrays.copyOfRange(cashflows, idx, cashflows.length);
		// If the settlement date falls into ex-div period, following
		// cashflow should be returned.
		// If the settlement date falls into last ex-div period, then
		// just the principal payment should be returned
		// (i.e. new cashflow object would have to be created)

		if (isExDividend(settlementDate)) {
			if (idx == (cashflows.length - 1)) {
				return new Cashflow[]{new Cashflow(maturity, redemption)};
			}

			cshf[0] = new Cashflow(cshf[0].getDate(), 0.0);
		}

		return cshf;

	}

	/**
	 * Returns index of the nearest cashflow, that has the date <b>on</b>
	 * or <b>after</b> the
	 * settlement date.
	 * @param settleDate settlement date
	 * @return Returns index of the nearest cashflow, that has the date on or after the
	 * settlement date. If the bond has no cashflows or settleDate is after maturity,
	 * -1 is returned.
	 */
	int getCashflowIndex(Date settleDate) {
		if (cashflows == null) {
			throw new RuntimeException("Array cashflows not initialized");
		}
		int settle = settleDate.toInt();
		int n = cashflows.length;
		if (n == 0 || settle > maturity.toInt()) {
			return -1;
		}
		if (settle <= firstCouponDate.toInt() || n == 1) {
			return 0;
		}
		if (settle > cashflows[n - 2].getDate().toInt()) {
			return n - 1;
		}

		double ndays = ((double) (cashflows[n - 2].getDate().toInt() - firstCouponDate.toInt()));
		double ni = ((n - 2.0) * ((double) (settle - firstCouponDate.toInt())) / ndays);
		int i = (int) (ni + 1.0);

		if (i >= n || i < 0.0) {
			throw new RuntimeException(
					"Internal error: getCashflowIndex failed to estimate initial value of the index");
		// TODO let's see if this ever happens,
		// if it does, we should add a look up through the complete
		// cashflow array...
		}

		if (i > 0 && cashflows[i - 1].getDate().toInt() < settle &&
				cashflows[i].getDate().toInt() >= settle) {
			return i;
		}

		while (i > 0 && cashflows[i].getDate().toInt() > settle) {
			i--;
		}

		while (i < (n - 1) && cashflows[i].getDate().toInt() < settle) {
			i++;
		}
		return i;
	}

	public double getNCashflows(Date settlementDate) {

		int index = getCashflowIndex(settlementDate);
		if (index == -1) {
			// there are no cashflows after the settlement date
			return 0;
		}
		return getNCashflows() - index;

	}

	/**
	 * Function <code>getNCashflows</code> returns the number of cashflows
	 * given bond pays out during its entire lifespan.
	 * @return number of cashflows, 0 means bond has no cashflows
	 */
	public int getNCashflows() {

		if (cashflows == null) {
			return 0;
		}

		return cashflows.length;

	}

	// Debt Security methods...
	public DebtSecurityClass getDebtSecurityClass() {
		return DebtSecurityClass.Bond;
	}

	public double getConventionalYield(Date settlementDate, double price) {

		return getConventionalYieldUnrounded(settlementDate, price);

	}

	public double getConventionalPrice(Date settlementDate, double yield) {

		return getConventionalPriceUnrounded(settlementDate, yield);

	}

	public double getConventionalYieldUnrounded(Date settlementDate, double yield ) {

		return getYield( settlementDate, yield );

	}

	public double getConventionalPriceUnrounded(Date settlementDate, double price ) {

		return getPrice( settlementDate, price );

	}

	public double getYield(Date settlementDate, double price) {

		Cashflow nextCF = getNextCashflow(settlementDate);

		if (nextCF.getDate().toInt() == maturity.toInt()) {
			return getSimpleYield(settlementDate, price);
		} else {
			return getYTM(settlementDate, price);
		}

	}

	public double getPrice(Date settlementDate, double yield) {

		Cashflow nextCF = getNextCashflow(settlementDate);

		if (nextCF.getDate().toInt() == maturity.toInt()) {
			return getPriceFromSimpleYield(settlementDate, yield);
		} else {
			return getPriceFromYTM(settlementDate, yield);
		}

	}

	public double getTrueYield(Date settlementDate, double price) {
		return getTrueYTM(settlementDate, price);
	}

	public double getTruePrice(Date settlementDate, double yield) {
		return getPriceFromTrueYTM(settlementDate, yield);
	}

	public double getTrueYTM(Date settlementDate, double price) {
		return getYTM(settlementDate, price, true);
	}

	public double getYTM(Date settlementDate, double price) {
		return getYTM(settlementDate, price, false);
	}

	public double getYTM(Date settlementDate, double price, boolean isTrueYield) {

		// Yield/price relationship is monotonous, hence we firstly need
		// to find some lower and higher estimates of yields.
		// We'll start with coupon and coupon increased by 1 percentage point
		// to get some sense of how steep the yield/price curve is.
		double yldEst = coupon -
				(price - redemption) / (100.0 * YearFrac(settlementDate, maturity,
				DaycountConvention.ACT_ACT));
		double yldEstLow = yldEst - 0.0050;
		double yldEstHigh = yldEst + 0.0050;

		double pxEst = getPriceFromYTM(settlementDate, yldEst, isTrueYield);
		double pxEstLow = getPriceFromYTM(settlementDate, yldEstLow, isTrueYield);
		double pxEstHigh = getPriceFromYTM(settlementDate, yldEstHigh, isTrueYield);

		Estimator estimator = null;
		double yldNewEst, pxNewEst;
		try {
			estimator = new YieldPriceEstimator(pxEst, yldEst,
					pxEstLow, yldEstLow, pxEstHigh, yldEstHigh);
		} catch (Exception e) {
			estimator = new EstimatorLinear(pxEst, yldEst,
					pxEst > price ? pxEstHigh : pxEstLow,
					pxEst > price ? yldEstHigh : yldEstLow);
		}

		yldNewEst = estimator.getEstimate(price);
		pxNewEst = getPriceFromYTM(settlementDate, yldNewEst, isTrueYield);
		int iterations = 0;

		while (Math.abs(price - pxNewEst) > YIELD_PRECISION) {

			if (pxNewEst > pxEstLow) {
				pxEstHigh = pxEst;
				yldEstHigh = yldEst;
				pxEst = pxEstLow;
				yldEst = yldEstLow;
				pxEstLow = pxNewEst;
				yldEstLow = yldNewEst;
			} else if (pxNewEst > pxEst) {
				pxEstHigh = pxEst;
				yldEstHigh = yldEst;
				pxEst = pxNewEst;
				yldEst = yldNewEst;
			} else if (pxNewEst > pxEstHigh) {
				pxEstLow = pxEst;
				yldEstLow = yldEst;
				pxEst = pxNewEst;
				yldEst = yldNewEst;
			} else {
				pxEstLow = pxEst;
				yldEstLow = yldEst;
				pxEst = pxEstHigh;
				yldEst = yldEstHigh;
				pxEstHigh = pxNewEst;
				yldEstHigh = yldNewEst;
			}
			try {
				estimator = new YieldPriceEstimator(pxEst, yldEst,
						pxEstLow, yldEstLow, pxEstHigh, yldEstHigh);
			} catch (Exception e) {
				estimator = new EstimatorLinear(pxEst, yldEst,
						pxEst > price ? pxEstHigh : pxEstLow,
						pxEst > price ? yldEstHigh : yldEstLow);
			}
			yldNewEst = estimator.getEstimate(price);
			pxNewEst = getPriceFromYTM(settlementDate, yldNewEst, isTrueYield);
			iterations++;
		}
		return yldNewEst;
	}

	public double getPriceFromYTM(Date settlementDate, double yield, boolean isTrueYield) {
		if (isTrueYield) {
			return getPriceFromTrueYTM(settlementDate, yield);
		} else {
			return getPriceFromYTM(settlementDate, yield);
		}
	}
	private Cashflow[] cachedCashflows;
	private Date cachedCashflowsSettlementDate;

	private void prepareCachedCashflows(Date settlementDate) {

		if (cachedCashflowsSettlementDate == null) {
			cachedCashflows = getCashflows(settlementDate);
			cachedCashflowsSettlementDate = settlementDate;
			return;
		}

		if (cachedCashflowsSettlementDate.toInt() == settlementDate.toInt()) {
			return;
		}

		cachedCashflows = getCashflows(settlementDate);
		cachedCashflowsSettlementDate = settlementDate;
	}

	public double getPriceFromYTM(Date settlementDate, double yield) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Settlement date must be before maturity" +
					" (settlement: " + settlementDate + ", maturity: " + maturity + ")");
		}

		double ytmFq = 1.0 + yield / compoundingFrequency;
		if (ytmFq < 0) {
			throw new IllegalArgumentException("Yield extremely negative, price calculation has failed (yield: " +
					yield + ")");
		}

		prepareCachedCashflows(settlementDate);

		if (cachedCashflows == null) {
			throw new RuntimeException("Bond has no cashflows, price/yield calculations cannot be performed (" +
					this.toString() + ")");
		}

		int index = 0;
		if (cachedCashflows[index].getDate().equals(settlementDate)) {
			index++;
		}

		if (isOddCoupon) {
			throw new UnsupportedOperationException("Price/Yield calculations for odd-coupon bonds not implemented");
		// TODO odd-coupon price/yield calculations
		}

		Date nextCpn = Coupons.getNextCouponDate(settlementDate, maturity, frequency, convention);

		double result = 0.0;
		double firstYearFrac = (double) NDays(settlementDate, nextCpn, convention) /
				(double) Coupons.getCouponDays(settlementDate, maturity, frequency, convention) / frequency;

		if (nextCpn.toInt() < cachedCashflows[index].getDate().toInt()) {
			firstYearFrac += Coupons.getNCoupons(nextCpn, cachedCashflows[index].getDate(), frequency, convention) / frequency;
		}
		double DF = Math.pow(ytmFq, firstYearFrac * compoundingFrequency);
		double DFmultiple = Math.pow(ytmFq, (double) compoundingFrequency / (double) frequency);

		int nCashflows = cachedCashflows.length;
		while (index < nCashflows) {
			result += cachedCashflows[index].getAmount() / DF;
			DF *= DFmultiple;
			index++;
		}

		result -= getAccruedInterestForYieldCalc(settlementDate);
		return result;

	/* original version before cachedCashflows array was implemented
	int index = getCashflowIndex(settlementDate);
	if (index == -1) {
	throw new RuntimeException("Bond has no cashflows, price/yield calculations cannot be performed (" +
	this.toString() + ")");
	}
	if (cashflows[index].getDate().equals(settlementDate)) {
	index++;
	}

	if (isOddCoupon) {
	throw new UnsupportedOperationException("Price/Yield calculations for odd-coupon bonds not implemented");
	// TODO odd-coupon price/yield calculations
	}

	Date nextCpn = Coupons.getNextCouponDate(settlementDate, maturity, frequency, convention);

	double result = 0.0;
	double firstYearFrac = (double) NDays(settlementDate, nextCpn, convention) /
	(double) Coupons.getCouponDays(settlementDate, maturity, frequency, convention) / frequency;

	if (nextCpn.toInt() < cashflows[index].getDate().toInt()) {
	firstYearFrac += Coupons.getNCoupons(nextCpn, cashflows[index].getDate(), frequency, convention) / frequency;
	}
	double DF = Math.pow(ytmFq, firstYearFrac * compoundingFrequency);
	double DFmultiple = Math.pow(ytmFq, (double) compoundingFrequency / (double) frequency);

	int ncashflows = cashflows.length;
	while (index < ncashflows) {
	result += cashflows[index].getAmount() / DF;
	DF *= DFmultiple;
	index++;
	}

	result -= getAccruedInterest(settlementDate);
	return result;
	 */
	}

	public double getPriceFromTrueYTM(Date settlementDate, double trueYTM) {

		// TODO modify code so that uses cachedCashflows array - necessary
		// for ex-div calculations!

		if (settlementDate.toInt() >= maturity.toInt()) {
			throw new IllegalArgumentException("Settlement date must be before maturity" +
					" (settlement: " + settlementDate + ", maturity: " + maturity + ")");
		}

		double ytmFq = 1.0 + trueYTM / compoundingFrequency;
		if (ytmFq < 0) {
			throw new IllegalArgumentException("Yield extremely negative, price calculation has failed (yield: " +
					trueYTM + ")");
		}

		int index = getCashflowIndex(settlementDate);
		if (index == -1) {
			throw new RuntimeException("Bond has no cashflows, price/yield calculations cannot be performed (" +
					this.toString() + ")");
		}
		if (cashflows[index].getDate().equals(settlementDate)) {
			index++;
		}

		if (isOddCoupon) {
			throw new UnsupportedOperationException("Price/Yield calculations for odd-coupon bonds not implemented");
		// TODO odd-coupon price/yield calculations
		}

		Calendar cal = getCalendar();
		if (cal == null) {
			cal = Calendar.getCalendar("empty");
		}

		Date nextCpn = Coupons.getNextCouponDate(settlementDate, maturity, frequency, convention);

		int ncashflows = cashflows.length;
		double[] diff = new double[cashflows.length];

		for (int i = index; i < ncashflows; i++) {
			Date cfDate = cashflows[i].getDate();
			Date bumpedDate = cal.getWorkDay(cfDate, 0);

			if (!cfDate.equals(bumpedDate)) {
				Date prevCoupon = Coupons.getPrevCouponDate(cfDate, maturity, frequency, convention);

				// next coupon date cannot be obtained by calling
				// Coupons.getNextCouponDate, as we'll need to call this function even
				// maturity date (which would throw an IllegalArgumentException)
				Date nextCoupon = cfDate.addMonths(12 / frequency);
				if (!convention.isNonEOM() && maturity.isMonthEnd()) {
					nextCoupon = nextCoupon.endOfMonth(0);
				}

				diff[i] = (bumpedDate.toInt() - cfDate.toInt()) /
						(double) (nextCoupon.toInt() - prevCoupon.toInt()) /
						(double) frequency;
			}
		}

		double result = 0.0;
		double firstYearFrac = (double) NDays(settlementDate, nextCpn, convention) /
				(double) Coupons.getCouponDays(settlementDate, maturity, frequency, convention) / frequency;

		if (nextCpn.toInt() < cashflows[index].getDate().toInt()) {
			firstYearFrac += Coupons.getNCoupons(nextCpn, cashflows[index].getDate(), frequency, convention) / frequency;
		}

		double DF = Math.pow(ytmFq, firstYearFrac * compoundingFrequency);
		double DFmultiple = Math.pow(ytmFq, (double) compoundingFrequency / (double) frequency);

		double amount;

		while (index < ncashflows) {

			amount = cashflows[index].getAmount();
			if (diff[index] > 0) {
				amount = amount / Math.pow(ytmFq, diff[index] * compoundingFrequency);
			}

			result += amount / DF;
			DF *= DFmultiple;
			index++;
		}

		result -= getAccruedInterestForYieldCalc(settlementDate);
		return result;
	}

	public double getPriceFromSimpleYield(Date settlementDate, double simpleYield) {

		if (getNCashflows(settlementDate) != 1) {
			throw new IllegalArgumentException("Simple yield/price calculations can only be " + //
					" performed for settlement dates from the last coupon period " + //
					" (settlement date: " + settlementDate.toString() + ").");
		}

		Cashflow nextCF = getNextCashflow(settlementDate);

		double nCouponPeriods = Coupons.getNCoupons(settlementDate, maturity, frequency, convention) - 1.0;
		double yearFrac = ((double) Coupons.getCouponDaysNC(settlementDate, maturity, frequency, convention) / (double) Coupons.getCouponDays(settlementDate, maturity, frequency, convention) + nCouponPeriods) / frequency;

		double fullPx = nextCF.getAmount() / (1.0 + simpleYield * yearFrac);
		double cleanPx = fullPx - getAccruedInterestForYieldCalc(settlementDate);

		return cleanPx;
	}

	public double getSimpleYield(Date settlementDate, double price) {

		if (getNCashflows(settlementDate) != 1) {
			throw new IllegalArgumentException("Simple yield/price calculations can only be " + //
					" performed for settlement dates from the last coupon period " + //
					" (settlement date: " + settlementDate.toString() + ").");
		}

		Cashflow nextCF = getNextCashflow(settlementDate);
		double fullPx = price + getAccruedInterestForYieldCalc(settlementDate);

		double nCouponPeriods = Coupons.getNCoupons(settlementDate, maturity, frequency, convention) - 1.0;
		double yearFrac = ((double) Coupons.getCouponDaysNC(settlementDate, maturity, frequency, convention) / (double) Coupons.getCouponDays(settlementDate, maturity, frequency, convention) + nCouponPeriods) / frequency;

		double simpleYield = (nextCF.getAmount() - fullPx) / fullPx / yearFrac;

		return simpleYield;
	}

	public double getMarketValue(Date settlementDate, double price) {

		return (price + getAccruedInterest(settlementDate));

	}

	/**
	 * Function getAccruedInterest returns accrued interest of the bond on the
	 * given date.
	 * @param valueDate Date for which the accrued interest should be computed.
	 * @return Returns accrued interest of the bond on the given value date.
	 * Returned value is expressed in percents, that is for a instead of 1%
	 * (or 0.01) value of 1 is returned. On coupon dates, zero is returned.
	 * Zero is also returned for all dates before the bond starts to accrue
	 * interest and also for maturity date.
	 * @throws IllegalArgumentException when <ul>
	 * <li>value date precedes issue date;</li>
	 * <li>value date follows maturity date.</li>
	 * </ul>
	 */
	public double getAccruedInterest(Date valueDate) {
		if (valueDate.toInt() < issueDate.toInt()) {
			throw new IllegalArgumentException("Value date can't be before issue date " +
					"(value date: " + valueDate + ", issue date: " + issueDate + ")");
		}
		if (valueDate.toInt() > maturity.toInt()) {
			throw new IllegalArgumentException("Value date can't be after maturity " +
					"(value date: " + valueDate + ", maturity: " + maturity + ")");
		}
		if (valueDate.toInt() < interestAccruesFrom.toInt() || valueDate.equals(maturity)) {
			return 0.0;
		}

		double accrued = Coupons.getAccruedInterest(valueDate, issueDate, maturity,
				interestAccruesFrom, firstCouponDate, coupon, 100.0, frequency, convention);

		if (isExDividend(valueDate)) {
			Cashflow cf = cashflows[getCashflowIndex(valueDate)];
			double nextCpn = cf.getAmount();
			if (cf.getDate().toInt() == maturity.toInt()) {
				nextCpn -= redemption;
			}
			return accrued - nextCpn;
		}

		return accrued;
	}

	double getAccruedInterestForYieldCalc(Date settlementDate) {
		return getAccruedInterest(settlementDate);
	}

	/**
	 * Returns whether the given settlement date is in the bond's ex-dividend period
	 * (or also know as ex-coupon period).
	 * @param settlementDate Settlement date
	 * @return Returns whether the given settlement date is in the bond's ex-dividend period
	 * (or also know as ex-coupon period). Bond.isExDividend returns:<ul>
	 * <li><code>false</code> if {@link #tradesExDividend} is false</li>
	 * <li><code>false</code> if {@link #tradesExDividend} is true and the settlement
	 * date is preceding the coupon payment date by more than {@link #exDividendDays}.</li>
	 * <li><code>true</code> if {@link #tradesExDividend} is true and the settlement
	 * date is preceding the coupon payment date by less than {@link #exDividendDays}.</li>
	 * </ul>
	 */
	public boolean isExDividend(Date settlementDate) {

		if (tradesExDividend == false) {
			return false;
		}

		int index = getCashflowIndex(settlementDate);

		if (index == -1) // -1 is returned only for settlement dates before issue date
		//    or past the maturity, in both cases it's clearly not an exDividend date
		{
			return false;
		}

		int nDaysBeforeCpn = cashflows[index].getDate().toInt() - settlementDate.toInt();

		if (nDaysBeforeCpn == 0) {
			return false;
		}
		if (nDaysBeforeCpn <= exDividendDays) {
			return true;
		}

		return false;
	}

	public double getDuration(Date settlementDate, double price) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			return 0.0;
		}

		double p0, p1, p2;
		double diff = 0.0001;
		double dur;

		double yield = getConventionalYieldUnrounded(settlementDate, price);

		p0 = getConventionalPriceUnrounded(settlementDate, yield);
		p1 = getConventionalPriceUnrounded(settlementDate, yield + diff);
		p2 = getConventionalPriceUnrounded(settlementDate, yield - diff);

		dur = ((-(p1 - p2) / (2 * diff)) / (p0 + getAccruedInterest(settlementDate))) * (1 + yield/frequency);

		return dur;
	}

	public double getMDuration(Date settlementDate, double price) {

		if (settlementDate.toInt() >= maturity.toInt()) {
			return 0.0;
		}

		double p0, p1, p2;
		double diff = 0.0001;
		double dur;

		double yield = getConventionalYieldUnrounded(settlementDate, price);

		p0 = getConventionalPriceUnrounded(settlementDate, yield);
		p1 = getConventionalPriceUnrounded(settlementDate, yield + diff);
		p2 = getConventionalPriceUnrounded(settlementDate, yield - diff);

		dur = ((-(p1 - p2) / (2 * diff)) / (p0 + getAccruedInterest(settlementDate))) * (1 + yield/frequency);

		return dur / (1.0 + yield / frequency);

	}
}

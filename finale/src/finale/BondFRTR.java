package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondFRTR extends Bond {

	private static final Date accruedInterestMethodChanged = Date.getValueOf(18, 4, 2005);

	BondFRTR(Builder parameters) {
		super(parameters);
	}

	@Override
	public String getBondType() {
		return "French government bond (FRTR)";
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {

		int roundDigits = 7;
		if (settlementDate.toInt() < accruedInterestMethodChanged.toInt()) {
			roundDigits = 5;
		}
		return Round(super.getAccruedInterest(settlementDate), roundDigits);
	}

	@Override
	public double getConventionalPrice(Date settlementDate, double yield) {

		return Round(super.getConventionalPrice(settlementDate, yield), 3);

	}

	@Override
	public double getConventionalYield(Date settlementDate, double price) {

		return Round(super.getConventionalYield(settlementDate, price), 5);

	}
}

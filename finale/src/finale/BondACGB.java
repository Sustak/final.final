package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondACGB extends Bond {

	BondACGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public void updateBond(Builder parameters) {
		if( parameters.tradesExDividendSet==false ) {
			parameters.tradesExDividend = true;
		}
		if( parameters.exDividendDaysSet==false ) {
			parameters.exDividendDays = 7;
		}
		super.updateBond(parameters);
	}

	@Override
	public String getBondType() {
		return "Australian government bond (ACGB)";
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {

		return Round(super.getAccruedInterest(settlementDate), 3);
	}

}

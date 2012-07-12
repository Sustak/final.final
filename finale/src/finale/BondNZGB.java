package finale;

import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondNZGB extends Bond {

	BondNZGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public void updateBond(Builder parameters) {
		if( parameters.tradesExDividendSet==false ) {
			parameters.tradesExDividend = true;
		}
		if( parameters.exDividendDaysSet==false ) {
			parameters.exDividendDays = 9;
		}
		super.updateBond(parameters);
	}

	@Override
	public String getBondType() {
		return "New Zealand government bond (NZGB)";
	}

}

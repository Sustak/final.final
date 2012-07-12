package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondCZGB extends Bond {

	BondCZGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public void updateBond(Builder parameters) {
		if (parameters.tradesExDividendSet == false) {
			parameters.tradesExDividend = true;
		}
		if (parameters.exDividendDaysSet == false) {
			parameters.exDividendDays = 30;
		}
		super.updateBond(parameters);
	}

	@Override
	public String getBondType() {
		return "Czech government bond (ACGB)";
	}

	@Override
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

		Date nextCpnDate = getCashflow(index).getDate();
		Date exDivDate = Date.EDate(nextCpnDate, -1);

		if (settlementDate.toInt() > exDivDate.toInt()) {
			if (settlementDate.toInt() == maturity.toInt()) {
				return false;
			} else {
				return true;
			}
		} else {
			return false;
		}
	}
}

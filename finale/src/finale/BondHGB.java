package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondHGB extends Bond {

	BondHGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public String getBondType() {
		return "Hungarian government bond (HGB)";
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {

		return Round(super.getAccruedInterest(settlementDate), 4);
	}
}

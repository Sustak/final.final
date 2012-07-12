package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondPOLGB extends Bond {

	BondPOLGB(Builder parameters) {
		super(parameters);
	}

	@Override
	public String getBondType() {
		return "Polish government bond (POLGB)";
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {
		return Round(super.getAccruedInterest(settlementDate), 3);
	}
}

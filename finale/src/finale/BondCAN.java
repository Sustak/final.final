package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondCAN extends Bond {

	BondCAN(Builder parameters) {
		super(parameters);
	}

	@Override
	public String getBondType() {
		return "Canadian government bond (CAN)";
	}

	@Override
	public double getAccruedInterest(Date settlementDate) {

		Date prevCpn = getPrevCouponDate(settlementDate);
		double nDaysSinceLastCpn = settlementDate.toInt()-prevCpn.toInt();
		if( nDaysSinceLastCpn>182 )
			nDaysSinceLastCpn = 181.5;

		return coupon/frequency*100.0*nDaysSinceLastCpn/182.5;
	}

	@Override
	double getAccruedInterestForYieldCalc( Date settlementDate ) {
		return super.getAccruedInterest(settlementDate);
	}
}

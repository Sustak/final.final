package finale;

import finale.utils.Date;

/**
 *
 * @author Marek Sestak
 * @version 1.0
 */
public interface InterestRateProjector {

    public double getRate( Date valueDate );
    public double getRate( Date valueDate, Date endDate );

}

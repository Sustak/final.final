package finale;

import finale.utils.Estimator;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class YieldPriceEstimator implements Estimator {

	private final double a,  b,  c;

	public YieldPriceEstimator(double p1, double y1,
			double p2, double y2, double p3, double y3) {

		try {

			double p3squared = p3 * p3;
			double p2squared = p2 * p2;
			double p1squared = p1 * p1;

			c = (p3squared * y3 - p3squared * y1 - (p2squared * y2 - p2squared * y1) *
					(1.0 - p3squared / p1squared) / (1.0 - p2squared / p1squared)) /
					(p3 - p3squared / p1 - (p2 - p2squared / p1) / (1.0 - p2squared / p1squared));

			b = (p2squared * y2 - p2squared * y1) / (1.0 - p2squared / p1squared) -
					(p2 - p2squared / p1) / (1.0 - p2squared / p1squared) * c;

			a = (p1squared * y1 - b - c * p1) / p1squared;

		} catch (Exception e) {
			throw new IllegalArgumentException("Provided yield/price combinations " +
					"couldn't be used to construct a YieldPriceEstimator");
		}
	}

	public double getEstimate(double price) {
		return a + c / price + b / (price * price);
	}
}

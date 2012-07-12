package finale.utils;

/**
 * Class LinearEstimator serves as a simple linear interpolation engine.
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class EstimatorLinear implements Estimator {

	private final double a;
	private final double b;

	public double getEstimate(double x) {
		return a + b * x;
	}

	public EstimatorLinear(
			double x1, double y1, double x2, double y2) {
		try {
			b = (y2 - y1) / (x2 - x1);
			a = y1 - b * x1;
		} catch (Exception e) {
			throw new IllegalArgumentException("Given combinations of " +
					"dependent and independent variables cannot be used to " +
					"construct linear interpolation estimator");
		}
	}
}

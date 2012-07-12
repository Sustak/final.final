package finale;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public enum BusinessDayConvention {

    NO_ADJUSTMENT, // 0
    AHEAD, // 1
    BACK, // 2
    MODIFIED_FOLLOWING, // 3
    MODIFIED_PRECEDING; // 4

    public int toInt() {
        return ordinal();
    }

    public static BusinessDayConvention toBusinessDayConvention(int code) {

        BusinessDayConvention result;
        try {
            result = values()[code];
        }
        catch( Exception ex ) {
            throw new IllegalArgumentException("Illegal numerical value (" +
                    code + ") representing business day convention.");
        }
        return result;

    }

}

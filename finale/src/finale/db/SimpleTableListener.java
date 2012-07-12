package finale.db;

/**
 *
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public interface SimpleTableListener {

    void recordInserted( int rowIndex );
    void recordDeletion( int rowIndex );
    void recordUpdated( int rowIndex );

}

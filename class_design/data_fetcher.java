import java.util.*;
import java.util.concurrent.*;

class Comment {
    private final String text;
    private final String userId;

    public Comment(String text, String userId) {
        this.text = text;
        this.userId = userId;
    }

    public String getText() {
        return text;
    }

    public String getUserId() {
        return userId;
    }
}


class User {
    private final String userId;
    private final String username;

    public User(String userId, String username) {
        this.userId = userId;
        this.username = username;
    }

    public String getUserId() {
        return userId;
    }

    public String getUsername() {
        return username;
    }

    @Override
    public String toString() {
        return username + "(" + userId + ")";
    }
}


/*
 * Assume interviewer provides this.
 *
 * DataFetcher:
 * 1. queues requests
 * 2. periodically flushes them
 * 3. makes one batch network request
 * 4. returns Futures for the results
 *
 * Important:
 * The backend batch response may come back
 * in a DIFFERENT ORDER from the requests.
 */
class DataFetcher {

    public Future<User> enqueue(String userId) {
        // Provided by interviewer.
        throw new UnsupportedOperationException();
    }
}


class UserService {

    private final DataFetcher dataFetcher;

    public UserService(DataFetcher dataFetcher) {
        this.dataFetcher = dataFetcher;
    }


    /*
     * OLD VERSION
     *
     * Correct, but inefficient because it makes
     * one network/database request per Comment.
     */
    public List<User> getUsersOld(
        List<Comment> comments
    ) throws Exception {

        List<User> result = new ArrayList<>();

        for (Comment comment : comments) {

            User user =
                getUser(comment.getUserId());

            result.add(user);
        }

        return result;
    }


    /*
     * NEW / FIXED VERSION
     *
     * Uses DataFetcher batching.
     */
    public List<User> getUsers(
        List<Comment> comments
    ) throws Exception {

        /*
         * STEP 1
         *
         * Enqueue every request first.
         *
         * Do NOT immediately call future.get(),
         * because we want DataFetcher to collect
         * these requests into a batch.
         */
        List<Future<User>> futures =
            new ArrayList<>();

        for (Comment comment : comments) {

            Future<User> future =
                dataFetcher.enqueue(
                    comment.getUserId()
                );

            futures.add(future);
        }


        /*
         * STEP 2
         *
         * Resolve every Future.
         *
         * Do NOT assume these Users are
         * associated with comments by position.
         */
        List<User> users =
            new ArrayList<>();

        for (Future<User> future : futures) {

            User user = future.get();

            users.add(user);
        }


        /*
         * STEP 3
         *
         * Build:
         *
         * userId -> User
         *
         * Now ordering no longer matters.
         */
        Map<String, User> userMap =
            new HashMap<>();

        for (User user : users) {

            userMap.put(
                user.getUserId(),
                user
            );
        }


        /*
         * STEP 4
         *
         * Iterate through ORIGINAL comments.
         *
         * The comments list has the ordering
         * that our result needs to preserve.
         */
        List<User> result =
            new ArrayList<>();

        for (Comment comment : comments) {

            String userId =
                comment.getUserId();

            User user =
                userMap.get(userId);

            result.add(user);
        }


        return result;
    }


    /*
     * Slow network/database call.
     * Assume provided by interviewer.
     */
    private User getUser(
        String userId
    ) throws Exception {

        throw new UnsupportedOperationException();
    }
}
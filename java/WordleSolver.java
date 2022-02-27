import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.*;
import java.util.stream.Collector;
import java.util.stream.Collectors;

public class WordleSolver {
    private static class WordScore {
        public WordScore(String w, int s) {
            this.word = w;
            this.score = s;
        }
        public String word;
        public int score;
        public int getScore() {
            return this.score;
        }

        public String toString() {
            return "(" + this.word + " -> " + this.score + ")";
        }
    }

    private static Logger logger = Logger.getGlobal();
    private static Set<String> wordsOf5; 
    private static int[] charFreqs = new int[26];
    private static List<WordScore> wordScores = new ArrayList<>();
    private static Map<Integer, Map<Integer, List<WordScore>>> byPositions = new HashMap<>(); 

    private static boolean containsChar(final String s, final char c) {
        return s.indexOf(c) != -1;
    }

    private static void getWordsOf5() throws IOException {
        wordsOf5 = Files.lines(Paths.get("/usr/share/dict/words"))
                        .filter(s -> s.length() == 5)
                        .map(String::toLowerCase)
                        .collect(Collectors.toSet());
        logger.info("Read " + wordsOf5.size() + " words...");
    }

    private static void populateFreqs() {
        wordsOf5.forEach(x -> {
            x.chars().forEach(c -> charFreqs[c - 'a']++);
        });
        wordsOf5.forEach(w -> 
            wordScores.add(
                new WordScore(
                    w, 
                    w.chars().map(c -> charFreqs[c - 'a']).sum())
            )
        );
        wordScores.sort(Comparator.comparing(WordScore::getScore).reversed());
    }

    private static void populatePositionMap() {
        for(int i = 0; i < 26; ++i) {
            byPositions.put(i, new HashMap<>());
            for(int j = 0; j < 5; ++j) {
                byPositions.get(i).put(j, new ArrayList<>());
            }
        }
        wordScores.forEach(w -> {
            w.word.chars().forEach(c -> {
                byPositions.get(c - 'a')
                           .get(w.word.indexOf(c, 0))
                           .add(w);
            });
        });
    }

    private static List<WordScore> popularWords(final int n) {
        return wordScores.stream().limit(n).collect(Collectors.toList());
    }

    private static WordScore randomPopularWord() {
        final int range = 100;
        return popularWords(range).get((int)Math.floor(Math.random() * range) - 1);
    }

    private static Set<String> guessBestNextWords(final String currentWord, final String hint) {

        // utilize hint to guess next best next words with score. B is still BLANK, G is GREEN, Y is YELLOW
        if(currentWord.isEmpty() || hint.isEmpty()) {
            return Set.of(randomPopularWord().word);
        } else {
            Map<Character, Integer> greenIndexes = new HashMap<>();
            Map<Character, Integer> yellowIndexes = new HashMap<>();
            Map<Character, Integer> blankIndexes = new HashMap<>();

            for(int i = 0; i < 5; ++i) {
                char c = hint.charAt(i);
                char cin = currentWord.charAt(i);
                if(c == 'b') {
                    if(!greenIndexes.containsKey(cin) && !yellowIndexes.containsKey(cin)) 
                        blankIndexes.put(cin, i);
                } else if(c == 'g') {
                    greenIndexes.put(cin, i);
                    if(blankIndexes.containsKey(cin)) blankIndexes.remove(cin);
                } else if(hint.charAt(i) == 'y') {
                    yellowIndexes.put(cin, i);
                    if(blankIndexes.containsKey(cin)) blankIndexes.remove(cin);
                }
            }

            // blank removal
            for(Map.Entry e : blankIndexes.entrySet()) {
                wordsOf5.removeIf(w -> containsChar(w, (char)e.getKey()));
            }

            logger.info("Remaining " + wordsOf5.size() + " words after blank removal!");

            // find all green words at position
            Set<String> greens = new HashSet<>();
            for(Map.Entry green : greenIndexes.entrySet()) {
                greens.addAll(byPositions.get((char)green.getKey()  - 'a')
                               .get(green.getValue()).stream()
                               .map(g -> g.word)
                               .collect(Collectors.toSet()));
            }
            
            System.out.println(greenIndexes);
            // retain only exact green matches
            StringBuilder regex = new StringBuilder();
            for(int i = 0; i < 5; i++) {
                Character c = currentWord.charAt(i);
                if(greenIndexes.containsKey(c) && i == greenIndexes.get(c)) regex.append(c);
                else regex.append(".");
            }

            System.out.println(regex.toString());
            final Pattern pattern = Pattern.compile(regex.toString());
            greens.removeIf(g -> {
                Matcher matcher = pattern.matcher(g);
                if(matcher.find()) return false;
                else return true;
            });

            // update master state again
            if(!greens.isEmpty()) wordsOf5.retainAll(greens);
            logger.info("Remaining " + wordsOf5.size() + " words after green inclusion!");
            
            // make sure they have yellows
            Set<Character> yellows = yellowIndexes.keySet();
            wordsOf5.removeIf(w -> {
                boolean remove = false;
                for(char c : yellows) {
                    if(containsChar(w, c)) continue;
                    else return true;
                }
                return remove;
            });
            logger.info("Remaining " + wordsOf5.size() + " words after yellow consideration!");
        }
        return wordsOf5;
    }

    public static void main(String[] args) {
        try {
            getWordsOf5();
        } catch(final Exception e) {
            logger.log(Level.SEVERE, "failed to laod dict", e);
        }
        populateFreqs();
        populatePositionMap();
        
        String initialState = "";
        String hint = "";
        String currentInput = "";
        System.out.println(guessBestNextWords(initialState.toLowerCase(), hint.toLowerCase()));

        while(true) {
            Scanner scanner = new Scanner(System.in);
            System.out.println("Enter current input: ");
            currentInput = scanner.nextLine();
            System.out.println("Enter hint: ");
            hint = scanner.nextLine();
            System.out.println(guessBestNextWords(currentInput.toLowerCase(), hint.toLowerCase()));
        }
    }
}
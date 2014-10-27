import Control.Applicative
import Data.STRef
import Control.Monad
import Control.Monad.ST
import System.IO
import qualified Data.Map as Map
import System.Environment (getArgs)


train' :: [String] -> Map.Map String Double
train' words = Map.map (/ (fromIntegral (length words))) $
        foldl (\dic word ->
            if Map.member word dic then
                Map.adjust (+1) word dic
            else
                Map.insert word 1 dic
        ) Map.empty words

train :: [String] -> Map.Map String Double
train lines = train' $ concat [words line ++ ["</s>"] | line <- lines]


main :: IO ()
main = do
    filename:_ <- getArgs
    handle <- openFile filename ReadMode
    lines <- lines <$> hGetContents handle
    print $ train lines

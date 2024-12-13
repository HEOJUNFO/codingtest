import java.util.HashSet;
import java.util.ArrayList;
import java.util.PriorityQueue;

class Solution
{
    class Edge implements Comparable<Edge> {
        int e;
        Edge left;
        Edge right;
        public Edge(int e) {
            this.e = e;
        }
        @Override
        public int compareTo(Edge o) {
            int thisSize = 0;
            int thatSize = 0;
            if (this.left != null) thisSize++;
            if (this.right != null) thisSize++;
            if (o.left != null) thatSize++;
            if (o.right != null) thatSize++;
            return thisSize - thatSize;
        }
    }
    class VInfo {
        int v;
        Edge e1;
        Edge e2;
        public VInfo(int v) {
            this.v = v;
        }
    }
    class EdgeGroup implements Comparable<EdgeGroup> {
        PriorityQueue<Edge> e1Set = new PriorityQueue<>();
        PriorityQueue<Edge> e2Set = new PriorityQueue<>();
        @Override
        public int compareTo(EdgeGroup o) {
            return this.e2Set.size() - this.e1Set.size() - o.e1Set.size() + o.e2Set.size();
        }
    }

    public void DFS(Edge curE, HashSet<Integer> searchedSet, EdgeGroup group, HashSet<Integer> e1Set, HashSet<Integer> e2Set) {
        if (searchedSet.contains(curE.e)) return;
        searchedSet.add(curE.e);

        if (e1Set.contains(curE.e)) {
            group.e1Set.add(curE);
        } else {
            group.e2Set.add(curE);
        }

        if (curE.left != null) {
            DFS(curE.left, searchedSet, group, e1Set, e2Set);
        }
        if (curE.right != null) {
            DFS(curE.right, searchedSet, group, e1Set, e2Set);
        }
    }

    public int[][] solution(int n, int m, int[] a, int[] b, int k, int m1, int m2, int[] e1, int[] e2)
    {
        ArrayList<int[]> theAnswer = new ArrayList<>();
        HashSet<Integer> e1Set = new HashSet<>(); // 인접한 간선이 있는지 확인해보려고
        HashSet<Integer> e2Set = new HashSet<>();

        for (int e : e1) {
            e1Set.add(e);
        }
        for (int e : e2) {
            e2Set.add(e);
        }

        // 둘 다 있을 경우 제거
        for (int i = 0; i < e1.length; i++) {
            if (e2Set.contains(e1[i])) {
                e1Set.remove(e1[i]);
                e2Set.remove(e1[i]);
            }
        }

        ArrayList<VInfo> vInfoList = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            vInfoList.add(new VInfo(i+1));
        }
        ArrayList<Edge> edgeList = new ArrayList<>();
        for (int i = 0; i < m; i++) {
            edgeList.add(new Edge(i+1));
        }

        for(int i = 0; i < a.length; i++) {
            VInfo vInfo = vInfoList.get(a[i]-1);
            Edge edge = edgeList.get(i);
            if (e1Set.contains(i+1)) {
                vInfo.e1 = edge;
            } else if (e2Set.contains(i+1)) {
                vInfo.e2 = edge;
            }
            vInfo = vInfoList.get(b[i]-1);
            if (e1Set.contains(i+1)) {
                vInfo.e1 = edge;
            } else if (e2Set.contains(i+1)) {
                vInfo.e2 = edge;
            }
        }

        for (VInfo vInfo : vInfoList) {
            if (vInfo.e1 != null && vInfo.e2 != null) {
                if (vInfo.e1.left == null) {
                    vInfo.e1.left = vInfo.e2;
                } else {
                    vInfo.e1.right = vInfo.e2;
                }
                if (vInfo.e2.left == null) {
                    vInfo.e2.left = vInfo.e1;
                } else {
                    vInfo.e2.right = vInfo.e1;
                }
            }
        }

        HashSet<Integer> searchedSet = new HashSet<>();
        PriorityQueue <EdgeGroup> groupList = new PriorityQueue<>();
        EdgeGroup group = null;
        // 군 정보 생성
        for (int e : e1Set) {
            if (searchedSet.contains(e)) continue;
            group = new EdgeGroup();
            DFS(edgeList.get(e-1), searchedSet, group, e1Set, e2Set);
            groupList.add(group);
        }

        for (int e : e2Set) {
            if (searchedSet.contains(e)) continue;
            group = new EdgeGroup();
            DFS(edgeList.get(e-1), searchedSet, group, e1Set, e2Set);
            groupList.add(group);
        }

        //System.out.print(groupList.size());

        // 그러면 어떻게 인접간선을 안만들고 삭제, 추가할까?
        int minSize = m1;
        HashSet<Integer> removeSet = new HashSet<>();
        while (groupList.size() > 0) {
            group = groupList.poll();
            while (group.e1Set.size() + group.e2Set.size() > 0) {

                if (!group.e2Set.isEmpty()) {
                    Edge ee = group.e2Set.poll();
                    if (removeSet.contains(ee.e)) break;
                    removeSet.add(ee.e);
                    if (ee.left != null) {
                        if (!removeSet.contains(ee.left.e)) {
                            theAnswer.add(new int[]{0, ee.left.e});
                            removeSet.add(ee.left.e);
                            minSize--;
                        }
                    }
                    if (ee.right != null) {
                        if (!removeSet.contains(ee.right.e)) {
                            theAnswer.add(new int[]{0, ee.right.e});
                            removeSet.add(ee.right.e);
                            minSize--;
                        }
                    }
                    theAnswer.add(new int[]{1, ee.e});
                    minSize++;

                }

                if (minSize < k-2 || group.e2Set.size() > 0) {
                    groupList.add(group);
                    continue;
                }

                if (!group.e1Set.isEmpty()) {
                    Edge ee = group.e1Set.poll();

                    if (removeSet.contains(ee.e)) break;
                    removeSet.add(ee.e);
                    theAnswer.add(new int[]{0, ee.e});
                    minSize--;
                }

            }
        }

        int[][] answer = new int[theAnswer.size()][2];
        theAnswer.toArray(answer);
        return answer;
    }
}
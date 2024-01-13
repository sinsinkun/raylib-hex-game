class BurstTimer {
  public:
    float duration;
    bool repeat = false;
    void update(float d);
    bool tick();
  private:
    float dt = 0;
    bool timeUp = false;
};